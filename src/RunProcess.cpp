/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/RunProcess.hpp"
#include "headers/system/Unix.hpp"
#include "headers/exception/SutExecutionException.hpp"
#include "headers/ErrorCodes.hpp"

#include <array>
#include <cstring>
#include <iostream>


namespace omtt
{

namespace
{

bool
IsParentProcess(int pid)
{
    return pid > 0;
}

std::string
ReadFromFd(int fd)
{
    std::string output;
    char buf[1024];
    int bytes;

    do {
        bytes = system::unix::Read(fd, buf, sizeof(buf) - 1);
        buf[bytes] = 0;
        output += buf;
    } while (bytes > 0);

    return output;
}

void
WriteToFd(int fd, const std::string_view &buf)
{
    std::string_view::size_type wrote = 0;

    while (wrote < buf.length()) {
        wrote += system::unix::Write(fd, buf.data() + wrote, buf.length() - wrote);
    }
}

void
RedirectPipe(int oldFd, int newFd)
{
    system::unix::Close(newFd);
    system::unix::DuplicateFd(oldFd, newFd);
}

static volatile sig_atomic_t childrenPid = 0;

void
KillChildProcessThenForwardToDefaultSignalHandler(int signum, siginfo_t *info, void *ucontext)
{
    std::cerr << "Received signal no " << signum << ", terminating SUT...\n";
    const int ret = kill(childrenPid, SIGKILL);
    if (ret < 0) {
        std::cerr << "SUT termination failed.";
    }
    else {
        (void) signal(signum, SIG_DFL);
        (void) raise(signum);
    }
}

void
SetSignalHandling(const int signum)
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = KillChildProcessThenForwardToDefaultSignalHandler;
    act.sa_flags = SA_SIGINFO;
    system::unix::SigAction(signum, &act, NULL);
}

void
SetDefaultSignalHandling(const int signum)
{
    system::unix::Signal(signum, SIG_DFL);
}

}

ProcessResults
RunProcess(const std::string &path,
           const std::vector<std::string> &options,
           const std::string_view &input)
{
    const int signals[] = {SIGHUP, SIGINT, SIGPIPE, SIGTERM, SIGUSR1, SIGUSR2};
    for (auto sig : signals) {
        SetSignalHandling(sig);
    }

    ProcessResults results;

    const auto toParentPipe = system::unix::MakePipe();
    const auto toChildPipe = system::unix::MakePipe();
    const auto toParentInternalErrorsPipe = system::unix::MakePipe(system::unix::PipeOptions::CLOSE_ON_EXEC);

    childrenPid = system::unix::Fork();

    if (IsParentProcess(childrenPid)) {
        system::unix::Close(toParentPipe.writeEnd);
        system::unix::Close(toChildPipe.readEnd);
        system::unix::Close(toParentInternalErrorsPipe.writeEnd);

        WriteToFd(toChildPipe.writeEnd, input);
        system::unix::Close(toChildPipe.writeEnd);

        results.output = ReadFromFd(toParentPipe.readEnd);
        results.exitCode = system::unix::ExitStatus(childrenPid);

        const std::string &internalErrors = ReadFromFd(toParentInternalErrorsPipe.readEnd);

        system::unix::Close(toParentPipe.readEnd);
        system::unix::Close(toParentInternalErrorsPipe.readEnd);

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
            WriteToFd(toParentInternalErrorsPipe.writeEnd, ex.what());
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
