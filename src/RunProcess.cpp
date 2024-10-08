/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "headers/RunProcess.hpp"
#include "headers/system/Unix.hpp"
#include "headers/exception/SutExecutionException.hpp"
#include "headers/exception/SignalReceivedException.hpp"
#include "headers/ErrorCodes.hpp"

#include <array>
#include <cstring>
#include <iostream>
#include <limits>


namespace omtt
{

void
RunProcessSignalHandler(const int signum, siginfo_t *info, void *ucontext);

namespace
{

constexpr int INTERNAL_TMP_BUFFER_SIZE = 1024;
using DataBuffer = std::array<char, INTERNAL_TMP_BUFFER_SIZE>;

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

int
ReadToBuffer(const int fd, DataBuffer &buf)
{
    const int bytes = system::unix::Read(fd, buf.data(), buf.size() - 1);
    buf.at(bytes) = 0;
    return bytes;
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
    const auto toParentErrorsPipe = system::unix::MakePipe();

    const auto childrenPid = system::unix::Fork();

    if (IsParentProcess(childrenPid)) {
        system::unix::Close(toParentPipe.writeEnd);
        system::unix::Close(toChildPipe.readEnd);
        system::unix::Close(toParentInternalErrorsPipe.writeEnd);
        system::unix::Close(toParentErrorsPipe.writeEnd);

        changeToNonBlocking(toChildPipe.writeEnd);

        struct pollfd fds[] = {
            {toParentPipe.readEnd, POLLIN, 0},
            {toChildPipe.writeEnd, POLLOUT, 0},
            {toParentInternalErrorsPipe.readEnd, POLLIN, 0},
            {toParentErrorsPipe.readEnd, POLLIN, 0}
        };

        std::string_view::size_type wroteToChild = 0;
        std::string internalErrors;
        DataBuffer buf;
        int processExitStatus;
        bool isProcessRunning = true;
        bool isToChildPipeWriteEndClosed = false;
        bool systemBuffersMayStillHaveData = false;

        do {
            systemBuffersMayStillHaveData = false;

            constexpr const int timeoutMs = 50;
            (void) system::unix::Poll(fds, sizeof(fds) / sizeof(fds[0]), timeoutMs);

            if (IsAbleToRead(fds[0])) {
                const int readBytes = ReadToBuffer(fds[0].fd, buf);
                if (readBytes > 0) {
                    results.output += buf.data();
                    systemBuffersMayStillHaveData = true;
                }
            }

            if (IsAbleToRead(fds[3])) {
                const int readBytes = ReadToBuffer(fds[3].fd, buf);
                if (readBytes > 0) {
                    results.errors += buf.data();
                    systemBuffersMayStillHaveData = true;
                }
            }

            if (IsAbleToWrite(fds[1]) && !IsAllDataWritten(wroteToChild, input)) {
                wroteToChild += system::unix::Write(toChildPipe.writeEnd,
                                                    input.data() + wroteToChild,
                                                    input.length() - wroteToChild,
                                                    system::unix::WriteOptions::IGNORE_EPIPE_EAGAIN);
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
                systemBuffersMayStillHaveData = true;
            }
        } while ((isProcessRunning || systemBuffersMayStillHaveData)
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
        system::unix::Close(toParentErrorsPipe.readEnd);

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
            system::unix::Close(toParentErrorsPipe.readEnd);

            RedirectPipe(toChildPipe.readEnd, static_cast<int>(system::unix::FdId::STDIN));
            RedirectPipe(toParentPipe.writeEnd, static_cast<int>(system::unix::FdId::STDOUT));
            RedirectPipe(toParentErrorsPipe.writeEnd, static_cast<int>(system::unix::FdId::STDERR));

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
