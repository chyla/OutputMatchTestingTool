/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/RunProcess.hpp"
#include "headers/system/Unix.hpp"
#include "headers/exception/SutExecutionException.hpp"
#include "headers/exception/SignalReceivedException.hpp"
#include "headers/ErrorCodes.hpp"

#include <array>
#include <cstring>
#include <iostream>


namespace omtt
{

void
RunProcessSignalHandler(const int signum, siginfo_t *info, void *ucontext);

namespace
{

const int INTERNAL_TMP_BUFFER_SIZE = 1024;

bool
IsParentProcess(const int pid)
{
    return pid > 0;
}

void
RedirectPipe(const int oldFd, const int newFd)
{
    system::unix::Close(newFd);
    system::unix::DuplicateFd(oldFd, newFd);
}

void
ReadToBuffer(const int fd, std::array<char, INTERNAL_TMP_BUFFER_SIZE> &buf)
{
    const int bytes = system::unix::Read(fd, buf.data(), buf.size() - 1);
    buf.at(bytes) = 0;
}

void
WriteAllDataToFd(const int fd, const std::string_view &buf)
{
    std::string_view::size_type wrote = 0;

    while (wrote < buf.length()) {
        wrote += system::unix::Write(fd, buf.data() + wrote, buf.length() - wrote);
    }
}

void
SetSignalHandling(const int signum)
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = RunProcessSignalHandler;
    act.sa_flags = SA_SIGINFO;
    system::unix::SigAction(signum, &act, NULL);
}

void
SetDefaultSignalHandling(const int signum)
{
    system::unix::Signal(signum, SIG_DFL);
}

void
IgnoreSignalHandling(const int signum)
{
    system::unix::Signal(signum, SIG_IGN);
}

bool
IsOperative(const struct pollfd &pfd)
{
    const bool isHup = pfd.revents & POLLHUP;
    const bool isErr = pfd.revents & POLLERR;
    return isHup == false && isErr == false;
}

int
ExitCode(const int wstatus)
{
    if (WIFEXITED(wstatus)) {
        return WEXITSTATUS(wstatus);
    }
    else {
        return std::numeric_limits<int>::max();
    }
}

bool
IsAbleToRead(const struct pollfd &pfd)
{
    return pfd.revents & POLLIN;
}

bool
IsAbleToWrite(const struct pollfd &pfd)
{
  return pfd.revents & POLLOUT
         && !(pfd.revents & POLLERR);
}

void
changeToNonBlocking(const int fd)
{
    int flags = system::unix::Fcntl(fd, F_GETFL, 0);
    system::unix::Fcntl(fd, F_SETFL, (flags | O_NONBLOCK));
}

bool
IsAllDataWritten(const std::string_view::size_type wroteToChild, const std::string_view &input)
{
    return wroteToChild == input.length();
}

volatile sig_atomic_t signalReceived;

}

void
RunProcessSignalHandler(const int signum, siginfo_t *info, void *ucontext)
{
    signalReceived = signum;
}

ProcessResults
RunProcess(const std::string &path,
           const std::vector<std::string> &options,
           const std::string_view &input)
{
    signalReceived = 0;
    const int signals[] = {SIGHUP, SIGINT, SIGTERM, SIGUSR1, SIGUSR2};
    for (auto sig : signals) {
        SetSignalHandling(sig);
    }
    IgnoreSignalHandling(SIGPIPE);

    ProcessResults results;

    const auto toParentPipe = system::unix::MakePipe();
    const auto toChildPipe = system::unix::MakePipe();
    const auto toParentInternalErrorsPipe = system::unix::MakePipe(system::unix::PipeOptions::CLOSE_ON_EXEC);

    const auto childrenPid = system::unix::Fork();

    if (IsParentProcess(childrenPid)) {
        system::unix::Close(toParentPipe.writeEnd);
        system::unix::Close(toChildPipe.readEnd);
        system::unix::Close(toParentInternalErrorsPipe.writeEnd);

        changeToNonBlocking(toChildPipe.writeEnd);

        struct pollfd fds[] = {
            {toParentPipe.readEnd, POLLIN, 0},
            {toChildPipe.writeEnd, POLLOUT, 0},
            {toParentInternalErrorsPipe.readEnd, POLLIN, 0}
        };

        std::string_view::size_type wroteToChild = 0;
        std::string internalErrors;
        std::array<char, INTERNAL_TMP_BUFFER_SIZE> buf;
        int processExitStatus;
        bool isProcessRunning = true;
        bool isToChildPipeWriteEndClosed = false;

        do {
            constexpr const int timeoutMs = 50;
            (void) system::unix::Poll(fds, sizeof(fds) / sizeof(fds[0]), timeoutMs);

            if (IsAbleToRead(fds[0])) {
                ReadToBuffer(fds[0].fd, buf);
                results.output += buf.data();
            }

            if (IsAbleToWrite(fds[1]) && !IsAllDataWritten(wroteToChild, input)) {
                wroteToChild += system::unix::Write(toChildPipe.writeEnd,
                                                    input.data() + wroteToChild,
                                                    input.length() - wroteToChild,
                                                    system::unix::WriteOptions::IGNORE_EPIPE);
            }

            if (isToChildPipeWriteEndClosed == false && IsAllDataWritten(wroteToChild, input)) {
                system::unix::Close(toChildPipe.writeEnd);
                isToChildPipeWriteEndClosed = true;
            }

            if (IsAbleToRead(fds[2])) {
                ReadToBuffer(fds[2].fd, buf);
                internalErrors += buf.data();
            }

            if (isProcessRunning) {
                const int pidOfProcessWithChangedStatus = system::unix::WaitPid(childrenPid, &processExitStatus, WNOHANG);
                isProcessRunning = (pidOfProcessWithChangedStatus == 0);
            }
        } while ((IsOperative(fds[0]) || IsAbleToRead(fds[0])
                  || (IsOperative(fds[1]) && !IsAllDataWritten(wroteToChild, input))
                  || isProcessRunning)
                 && signalReceived == 0);

        if (signalReceived) {
            system::unix::Kill(childrenPid, SIGKILL);
            throw exception::SignalReceivedException(signalReceived);
        }

        if (isToChildPipeWriteEndClosed == false) {
            system::unix::Close(toChildPipe.writeEnd);
        }

        system::unix::Close(toParentPipe.readEnd);
        system::unix::Close(toParentInternalErrorsPipe.readEnd);

        results.exitCode = ExitCode(processExitStatus);

        if (internalErrors.length() > 0) {
            throw exception::SutExecutionException("during SUT execution: " + internalErrors);
        }
    }
    else {
        try {
            system::unix::Close(toParentPipe.readEnd);
            system::unix::Close(toChildPipe.writeEnd);
            system::unix::Close(toParentInternalErrorsPipe.readEnd);

            RedirectPipe(toChildPipe.readEnd, static_cast<int>(system::unix::FdId::STDIN));
            RedirectPipe(toParentPipe.writeEnd, static_cast<int>(system::unix::FdId::STDOUT));

            system::unix::Exec(path, options);
        }
        catch (const std::exception &ex) {
            WriteAllDataToFd(toParentInternalErrorsPipe.writeEnd, ex.what());
            system::unix::Terminate(FATAL_ERROR);
            throw;
        }
    }

    for (auto sig : signals) {
        SetDefaultSignalHandling(sig);
    }

    return results;
}

}  // omtt
