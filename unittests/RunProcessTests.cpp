/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/RunProcess.hpp"
#include "headers/ErrorCodes.hpp"
#include "headers/exception/SutExecutionException.hpp"
#include "headers/exception/SignalReceivedException.hpp"
#include "headers/system/exception/SystemException.hpp"
#include "unittests/system/UnixFake.hpp"

#include "unittests/test_framework.hpp"

#include <iterator>
#include <string>

#ifndef __W_EXITCODE
int __W_EXITCODE(int ret, int sig)
{
	return ret << 8 | sig;
}
#endif

namespace omtt
{

extern void
RunProcessSignalHandler(const int signum, siginfo_t *info, void *ucontext);

auto &systemFake = system::unix::GlobalFake();

constexpr const char *exampleBinaryPath = "/bin/example";
constexpr int anyChildProcessId = 1347;

const std::string emptyInput;
const std::string nonImportantEmptyInput;
const std::string nonImportantNonEmptyInput = "XYZ";
const std::vector<std::string> emptyRunProcessArguments = {};


TEST_GROUP("Child Process Exit Code")
{
    system::unix::ResetGlobalFake();

    systemFake.MakePipeAction = [](const system::unix::PipeOptions option) -> system::unix::Pipe { return {0, 0}; };
    systemFake.ForkAction = []() { return anyChildProcessId; };
    systemFake.CloseAction = [](int) {};
    systemFake.WriteAction = [](int, const void *, size_t, system::unix::WriteOptions) -> ssize_t { return 0; };
    systemFake.ReadAction = [](int fd, void *buf, size_t count) -> ssize_t { return 0; };
    systemFake.SigAction = [](int, const struct sigaction*, struct sigaction*) {};
    systemFake.Signal = [](int, sighandler_t){};
    systemFake.FcntlAction = [](int, int, int) { return 0; };


    UNIT_TEST("Should return correct exit code when process is not running and fds have status HUP")
    {
        const int expectedExitCode = 143;

        systemFake.PollAction = [](struct pollfd *fds, nfds_t nfds, int timeout) {
                                    fds[0].revents = POLLHUP;
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.WaitPidAction = [&](int pid, int *wstatus, int options) {
                                         *wstatus = __W_EXITCODE(expectedExitCode, 0);
                                         return pid;
                                      };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

        CHECK(results.exitCode == expectedExitCode);
    }

    UNIT_TEST("Should return correct exit code when process fds hangs up but it is still running")
    {
        const int expectedExitCode = 143;

        systemFake.PollAction = [](struct pollfd *fds, nfds_t nfds, int timeout) {
                                    fds[0].revents = POLLHUP;
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.WaitPidAction = [&, run = 0](int pid, int *wstatus, int options) mutable {
                                       ++run;
                                       if (run == 1) {
                                           *wstatus = 0;
                                           return 0;
                                       }
                                       else {
                                           *wstatus = __W_EXITCODE(expectedExitCode, 0);
                                           return pid;
                                       }
                                   };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, emptyInput);

        CHECK(results.exitCode == expectedExitCode);
    }
}


TEST_GROUP("Read Child Process Output")
{
    system::unix::ResetGlobalFake();

    systemFake.MakePipeAction = [](const system::unix::PipeOptions option) -> system::unix::Pipe { return {0, 0}; };
    systemFake.ForkAction = []() { return anyChildProcessId; };
    systemFake.CloseAction = [](int) {};
    systemFake.WriteAction = [](int, const void *, size_t, system::unix::WriteOptions) -> ssize_t { return 0; };
    systemFake.WaitPidAction = [run = 0](int pid, int *wstatus, int options) mutable {
                                    run++;
                                    if (run == 10) {
                                        return 1;
                                    }
                                    else {
                                        return 0;
                                    }
                               };
    systemFake.SigAction = [](int, const struct sigaction*, struct sigaction*) {};
    systemFake.Signal = [](int, sighandler_t){};
    systemFake.FcntlAction = [](int, int, int) { return 0; };


    UNIT_TEST("Should return empty output when child output is empty")
    {
        systemFake.PollAction = [](struct pollfd *fds, nfds_t nfds, int timeout) {
                                    fds[0].revents = POLLHUP;
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.ReadAction = [](int fd, void *buf, size_t count) -> ssize_t {
                                    return 0;
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

        CHECK(results.output == "");
    }

    UNIT_TEST("Should return empty output when child output is empty and POLLIN is still set (FreeBSD)")
    {
        systemFake.PollAction = [](struct pollfd *fds, nfds_t nfds, int timeout) {
                                    fds[0].revents = POLLHUP | POLLIN;
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.ReadAction = [](int fd, void *buf, size_t count) -> ssize_t {
                                    return 0;
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

        CHECK(results.output == "");
    }

    UNIT_TEST("Should return empty output when child output is empty and only POLLIN is set after child exit (Cygwin)")
    {
        systemFake.WaitPidAction = [](int pid, int *wstatus, int options) {
                                    return 1;
                                };
        systemFake.PollAction = [](struct pollfd *fds, nfds_t nfds, int timeout) {
                                    fds[0].revents = POLLIN;
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.ReadAction = [](int fd, void *buf, size_t count) -> ssize_t {
                                    return 0;
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

        CHECK(results.output == "");
    }

    UNIT_TEST("Should return process output when output is short")
    {
        const std::string expectedProcessOutput = "ShortProcessOutput";

        systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                    ++run;
                                    if (run == 1) {
                                        fds[0].revents = POLLIN;
                                    }
                                    else {
                                        fds[0].revents = POLLHUP;
                                    }
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.ReadAction = [&, run = 0](int fd, void *buf, size_t count) mutable -> ssize_t {
                                    ++run;
                                    auto dest = static_cast<char*>(buf);
                                    if (run == 1) {
                                        std::copy(expectedProcessOutput.begin(), expectedProcessOutput.end(), dest);
                                        return expectedProcessOutput.length();
                                    }
                                    else {
                                        return 0;
                                    }
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

        CHECK(results.output == expectedProcessOutput);
    }

    UNIT_TEST("Should return process output when output is long and requires multiple read calls, but sometimes read returns nothing")
    {
        const std::string expedtedProcessOutputPart1 = "L";
        const std::string expedtedProcessOutputPart2 = "ongProcess";
        const std::string expedtedProcessOutputPart3 = "Output";
        const std::string expectedProcessOutput = expedtedProcessOutputPart1
                                                  + expedtedProcessOutputPart2
                                                  + expedtedProcessOutputPart3;

        systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                    ++run;
                                    if (run == 1) {
                                        fds[0].revents = POLLIN;
                                    }
                                    else if (run == 2) {
                                        fds[0].revents = POLLIN;
                                    }
                                    else if (run == 3) {
                                        fds[0].revents = POLLIN;
                                    }
                                    else if (run == 4) {
                                        fds[0].revents = POLLIN;
                                    }
                                    else if (run == 5) {
                                        fds[0].revents = POLLIN;
                                    }
                                    else if (run == 6) {
                                        fds[0].revents = POLLIN;
                                    }
                                    else {
                                        fds[0].revents = POLLHUP | POLLIN;
                                    }
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.ReadAction = [&, run = 0](int fd, void *buf, size_t count) mutable -> ssize_t {
                                    ++run;
                                    auto dest = static_cast<char*>(buf);
                                    if (run == 1) {
                                        std::copy(expedtedProcessOutputPart1.begin(), expedtedProcessOutputPart1.end(), dest);
                                        return expedtedProcessOutputPart1.length();
                                    }
                                    else if (run == 2) {
                                        return 0;
                                    }
                                    else if (run == 3) {
                                        return 0;
                                    }
                                    else if (run == 4) {
                                        std::copy(expedtedProcessOutputPart2.begin(), expedtedProcessOutputPart2.end(), dest);
                                        return expedtedProcessOutputPart2.length();
                                    }
                                    else if (run == 5) {
                                        return 0;
                                    }
                                    else if (run == 6) {
                                        std::copy(expedtedProcessOutputPart3.begin(), expedtedProcessOutputPart3.end(), dest);
                                        return expedtedProcessOutputPart3.length();
                                    }
                                    else {
                                        return 0;
                                    }
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

        CHECK(results.output == expectedProcessOutput);
    }

    UNIT_TEST("Should return process output when output is long and requires multiple read calls, but there is not always POLLIN set")
    {
        const std::string expedtedProcessOutputPart1 = "L";
        const std::string expedtedProcessOutputPart2 = "ongProcess";
        const std::string expedtedProcessOutputPart3 = "Output";
        const std::string expectedProcessOutput = expedtedProcessOutputPart1
                                                  + expedtedProcessOutputPart2
                                                  + expedtedProcessOutputPart3;

        systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                    ++run;
                                    if (run == 1) {
                                        fds[0].revents = POLLIN;
                                    }
                                    else if (run == 2) {
                                        fds[0].revents = 0;
                                    }
                                    else if (run == 3) {
                                        fds[0].revents = POLLIN;
                                    }
                                    else if (run == 4) {
                                        fds[0].revents = POLLIN;
                                    }
                                    else if (run == 5) {
                                        fds[0].revents = 0;
                                    }
                                    else if (run == 6) {
                                        fds[0].revents = 0;
                                    }
                                    else {
                                        fds[0].revents = POLLHUP;
                                    }
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.ReadAction = [&, run = 0](int fd, void *buf, size_t count) mutable -> ssize_t {
                                    ++run;
                                    auto dest = static_cast<char*>(buf);
                                    if (run == 1) {
                                        std::copy(expedtedProcessOutputPart1.begin(), expedtedProcessOutputPart1.end(), dest);
                                        return expedtedProcessOutputPart1.length();
                                    }
                                    else if (run == 2) {
                                        std::copy(expedtedProcessOutputPart2.begin(), expedtedProcessOutputPart2.end(), dest);
                                        return expedtedProcessOutputPart2.length();
                                    }
                                    else if (run == 3) {
                                        std::copy(expedtedProcessOutputPart3.begin(), expedtedProcessOutputPart3.end(), dest);
                                        return expedtedProcessOutputPart3.length();
                                    }
                                    else {
                                        return 0;
                                    }
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

        CHECK(results.output == expectedProcessOutput);
    }

    UNIT_TEST("Should return process output when output is long and requires multiple read calls")
    {
        const std::string expedtedProcessOutputPart1 = "L";
        const std::string expedtedProcessOutputPart2 = "ongProcess";
        const std::string expedtedProcessOutputPart3 = "Output";
        const std::string expectedProcessOutput = expedtedProcessOutputPart1
                                                  + expedtedProcessOutputPart2
                                                  + expedtedProcessOutputPart3;

        systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                    ++run;
                                    if (run <= 3) {
                                        fds[0].revents = POLLIN;
                                    }
                                    else {
                                        fds[0].revents = POLLHUP;
                                    }
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.ReadAction = [&, run = 0](int fd, void *buf, size_t count) mutable -> ssize_t {
                                    ++run;
                                    auto dest = static_cast<char*>(buf);
                                    if (run == 1) {
                                        std::copy(expedtedProcessOutputPart1.begin(), expedtedProcessOutputPart1.end(), dest);
                                        return expedtedProcessOutputPart1.length();
                                    }
                                    else if (run == 2) {
                                        std::copy(expedtedProcessOutputPart2.begin(), expedtedProcessOutputPart2.end(), dest);
                                        return expedtedProcessOutputPart2.length();
                                    }
                                    else if (run == 3) {
                                        std::copy(expedtedProcessOutputPart3.begin(), expedtedProcessOutputPart3.end(), dest);
                                        return expedtedProcessOutputPart3.length();
                                    }
                                    else {
                                        return 0;
                                    }
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

        CHECK(results.output == expectedProcessOutput);
    }

    UNIT_TEST("Should return process output when children closed stdout and exited but there are still some data to read from the system buffer, POLLIN is NOT SET when there is no more data (Linux)")
    {
        const std::string expedtedProcessOutputPart1 = "Long";
        const std::string expedtedProcessOutputPart2 = "Proc";
        const std::string expedtedProcessOutputPart3 = "essOutput";
        const std::string expectedProcessOutput = expedtedProcessOutputPart1
                                                  + expedtedProcessOutputPart2
                                                  + expedtedProcessOutputPart3;

        systemFake.WaitPidAction = [](int pid, int *wstatus, int options) {
                                    return 1;
                                };
        systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                    ++run;
                                    if (run <= 3) {
                                        fds[0].revents = POLLHUP | POLLIN;
                                    }
                                    else {
                                        fds[0].revents = POLLHUP;
                                    }
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.ReadAction = [&, run = 0](int fd, void *buf, size_t count) mutable -> ssize_t {
                                    ++run;
                                    auto dest = static_cast<char*>(buf);
                                    if (run == 1) {
                                        std::copy(expedtedProcessOutputPart1.begin(), expedtedProcessOutputPart1.end(), dest);
                                        return expedtedProcessOutputPart1.length();
                                    }
                                    else if (run == 2) {
                                        std::copy(expedtedProcessOutputPart2.begin(), expedtedProcessOutputPart2.end(), dest);
                                        return expedtedProcessOutputPart2.length();
                                    }
                                    else if (run == 3) {
                                        std::copy(expedtedProcessOutputPart3.begin(), expedtedProcessOutputPart3.end(), dest);
                                        return expedtedProcessOutputPart3.length();
                                    }
                                    else {
                                        return 0;
                                    }
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

        CHECK(results.output == expectedProcessOutput);
    }

    UNIT_TEST("Should return process output when children closed stdout and exited but there are still some data to read from the system buffer, POLLIN is STILL SET when there is no data (FreeBSD)")
    {
        const std::string expedtedProcessOutputPart1 = "Long";
        const std::string expedtedProcessOutputPart2 = "Proc";
        const std::string expedtedProcessOutputPart3 = "essOutput";
        const std::string expectedProcessOutput = expedtedProcessOutputPart1
                                                  + expedtedProcessOutputPart2
                                                  + expedtedProcessOutputPart3;

        systemFake.WaitPidAction = [](int pid, int *wstatus, int options) {
                                    return 1;
                                };
        systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                    ++run;
                                    fds[0].revents = POLLHUP | POLLIN;
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.ReadAction = [&, run = 0](int fd, void *buf, size_t count) mutable -> ssize_t {
                                    ++run;
                                    auto dest = static_cast<char*>(buf);
                                    if (run == 1) {
                                        std::copy(expedtedProcessOutputPart1.begin(), expedtedProcessOutputPart1.end(), dest);
                                        return expedtedProcessOutputPart1.length();
                                    }
                                    else if (run == 2) {
                                        std::copy(expedtedProcessOutputPart2.begin(), expedtedProcessOutputPart2.end(), dest);
                                        return expedtedProcessOutputPart2.length();
                                    }
                                    else if (run == 3) {
                                        std::copy(expedtedProcessOutputPart3.begin(), expedtedProcessOutputPart3.end(), dest);
                                        return expedtedProcessOutputPart3.length();
                                    }
                                    else {
                                        return 0;
                                    }
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

        CHECK(results.output == expectedProcessOutput);
    }

    UNIT_TEST("Should return process output when children closed stdout and exited but there are still some data to read from the system buffer, only POLLIN is SET when child exit (Cygwin)")
    {
        const std::string expedtedProcessOutputPart1 = "Long";
        const std::string expedtedProcessOutputPart2 = "Proc";
        const std::string expedtedProcessOutputPart3 = "essOutput";
        const std::string expectedProcessOutput = expedtedProcessOutputPart1
                                                  + expedtedProcessOutputPart2
                                                  + expedtedProcessOutputPart3;

        systemFake.WaitPidAction = [](int pid, int *wstatus, int options) {
                                    return 1;
                                };
        systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                    ++run;
                                    fds[0].revents = POLLIN;
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.ReadAction = [&, run = 0](int fd, void *buf, size_t count) mutable -> ssize_t {
                                    ++run;
                                    auto dest = static_cast<char*>(buf);
                                    if (run == 1) {
                                        std::copy(expedtedProcessOutputPart1.begin(), expedtedProcessOutputPart1.end(), dest);
                                        return expedtedProcessOutputPart1.length();
                                    }
                                    else if (run == 2) {
                                        std::copy(expedtedProcessOutputPart2.begin(), expedtedProcessOutputPart2.end(), dest);
                                        return expedtedProcessOutputPart2.length();
                                    }
                                    else if (run == 3) {
                                        std::copy(expedtedProcessOutputPart3.begin(), expedtedProcessOutputPart3.end(), dest);
                                        return expedtedProcessOutputPart3.length();
                                    }
                                    else {
                                        return 0;
                                    }
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

        CHECK(results.output == expectedProcessOutput);
    }

    UNIT_TEST("Should return process output when child doesn't return any data at begining, then quickly returns data and exits")
    {
        const std::string expedtedProcessOutputPart1 = "Long";
        const std::string expedtedProcessOutputPart2 = "Proc";
        const std::string expedtedProcessOutputPart3 = "essOutput";
        const std::string expectedProcessOutput = expedtedProcessOutputPart1
                                                  + expedtedProcessOutputPart2
                                                  + expedtedProcessOutputPart3;

        systemFake.WaitPidAction = [](int pid, int *wstatus, int options) {
                                    return 1;
                                };
        systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                    ++run;
                                    fds[0].revents = POLLIN;
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.ReadAction = [&, run = 0](int fd, void *buf, size_t count) mutable -> ssize_t {
                                    ++run;
                                    auto dest = static_cast<char*>(buf);
                                    if (run == 1) {
                                        return 0;
                                    }
                                    else if (run == 2) {
                                        std::copy(expedtedProcessOutputPart1.begin(), expedtedProcessOutputPart1.end(), dest);
                                        return expedtedProcessOutputPart1.length();
                                    }
                                    else if (run == 3) {
                                        std::copy(expedtedProcessOutputPart2.begin(), expedtedProcessOutputPart2.end(), dest);
                                        return expedtedProcessOutputPart2.length();
                                    }
                                    else if (run == 4) {
                                        std::copy(expedtedProcessOutputPart3.begin(), expedtedProcessOutputPart3.end(), dest);
                                        return expedtedProcessOutputPart3.length();
                                    }
                                    else {
                                        return 0;
                                    }
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

        CHECK(results.output == expectedProcessOutput);
    }

}


TEST_GROUP("Write To Child Process")
{
    system::unix::ResetGlobalFake();

    systemFake.MakePipeAction = [](const system::unix::PipeOptions option) -> system::unix::Pipe { return {0, 0}; };
    systemFake.ForkAction = []() { return anyChildProcessId; };
    systemFake.CloseAction = [](int) {};
    systemFake.ReadAction = [](int, const void *, size_t) -> ssize_t { return 0; };
    systemFake.WaitPidAction = [run = 0](int pid, int *wstatus, int options) mutable {
                                    run++;
                                    if (run == 10) {
                                        return 1;
                                    }
                                    else {
                                        return 0;
                                    }
                               };
    systemFake.SigAction = [](int, const struct sigaction*, struct sigaction*) {};
    systemFake.Signal = [](int, sighandler_t){};
    systemFake.FcntlAction = [](int, int, int) { return 0; };


    UNIT_TEST("Should not pass input when empty input string is given")
    {
       systemFake.PollAction = [](struct pollfd *fds, nfds_t nfds, int timeout) {
                                     fds[0].revents = POLLHUP;
                                     fds[1].revents = POLLOUT;
                                     fds[2].revents = POLLHUP;
                                     return 0;
                                };
        systemFake.WriteAction = [&](int fd, const void *buf, size_t count, system::unix::WriteOptions) -> ssize_t {
                                     throw std::logic_error("Write function shouldn't be called.");
                                 };
        const std::string emptyProcessInput = "";

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, emptyProcessInput);
    }

    UNIT_TEST("Should pass input when input string is short")
    {
        const std::string input = "ShortProcessInput";
        std::string result;
        std::vector<ssize_t> counts;

        systemFake.PollAction = [](struct pollfd *fds, nfds_t nfds, int timeout) {
                                    fds[0].revents = POLLHUP;
                                    fds[1].revents = POLLOUT;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.WriteAction = [&](int fd, const void *buf, size_t count, system::unix::WriteOptions) -> ssize_t {
                                    auto src = static_cast<const char*>(buf);
                                    std::string s(src, count);
                                    counts.push_back(count);
                                    result += s;
                                    return s.length();
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, input);

        CHECK(result == input);
        CHECK(counts.size() == 1);
        CHECK(counts.at(0) == input.length());
    }

    UNIT_TEST("Should pass input when input string is long and requires multiple write call")
    {
        const std::string input = "ShortProcessInput";
        constexpr int sizeOfPart3 = 1;
        constexpr int sizeOfPart2 = 10;
        const int sizeOfPart1 = input.length() - sizeOfPart3 - sizeOfPart2;

        std::string result;
        std::vector<ssize_t> counts;

        systemFake.PollAction = [](struct pollfd *fds, nfds_t nfds, int timeout) {
                                    fds[0].revents = POLLHUP;
                                    fds[1].revents = POLLOUT;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.WriteAction = [&, run = 0](int fd, const void *buf, size_t count, system::unix::WriteOptions) mutable -> ssize_t {
                                    ++run;
                                    auto src = static_cast<const char*>(buf);

                                    counts.push_back(count);

                                    if (run == 1) {
                                        std::copy_n(src, sizeOfPart1, std::back_inserter(result));
                                        return sizeOfPart1;
                                    }
                                    else if (run == 2) {
                                        std::copy_n(src, sizeOfPart2, std::back_inserter(result));
                                        return sizeOfPart2;
                                    }
                                    else if (run == 3) {
                                        std::copy_n(src, sizeOfPart3, std::back_inserter(result));
                                        return sizeOfPart3;
                                    }
                                    else {
                                        return 0;
                                    }
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, input);

        CHECK(result == input);
        CHECK(counts.size() == 3);
        CHECK(counts.at(0) == input.length());
        CHECK(counts.at(1) == input.length() - sizeOfPart1);
        CHECK(counts.at(2) == input.length() - sizeOfPart1 - sizeOfPart2);
    }

    UNIT_TEST("Should ignore remaining input string when child closes stdin")
    {
        const std::string inputPart1 = "Short";
        const std::string inputPart2 = "Process";
        const std::string inputPart3 = "Input";
        const std::string input = inputPart1 + inputPart2 + inputPart3;

        std::string result;
        std::vector<ssize_t> counts;

        systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                    ++run;
                                    fds[0].revents = POLLHUP;
                                    if (run == 1 || run == 2) {
                                        fds[1].revents = POLLOUT;
                                    }
                                    else {
                                        fds[1].revents = POLLHUP;
                                    }
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.WriteAction = [&, run = 0](int fd, const void *buf, size_t count, system::unix::WriteOptions) mutable -> ssize_t {
                                    ++run;
                                    auto src = static_cast<const char*>(buf);

                                    counts.push_back(count);

                                    if (run == 1) {
                                        std::copy_n(src, inputPart1.length(), std::back_inserter(result));
                                        return inputPart1.length();
                                    }
                                    if (run == 2) {
                                        std::copy_n(src, inputPart2.length(), std::back_inserter(result));
                                        return inputPart2.length();
                                    }
                                    else {
                                        throw std::logic_error("Write function shouldn't be called.");
                                    }
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, input);

        CHECK(result == inputPart1 + inputPart2);
        CHECK(counts.size() == 2);
        CHECK(counts.at(0) == input.length());
        CHECK(counts.at(1) == input.length() - inputPart1.length());
    }

    UNIT_TEST("Should ignore remaining input string when POLLERR is set with POLLOUT (this can occour when children exits or crash before all data are written)")
    {
        const std::string inputPart1 = "Short";
        const std::string inputPart2 = "Process";
        const std::string inputPart3 = "Input";
        const std::string input = inputPart1 + inputPart2 + inputPart3;

        std::string result;
        std::vector<ssize_t> counts;

        systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                    ++run;
                                    fds[0].revents = POLLHUP;
                                    if (run == 1 || run == 2) {
                                        fds[1].revents = POLLOUT;
                                    }
                                    else {
                                        fds[1].revents = POLLERR | POLLOUT;
                                    }
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.WriteAction = [&, run = 0](int fd, const void *buf, size_t count, system::unix::WriteOptions) mutable -> ssize_t {
                                    ++run;
                                    auto src = static_cast<const char*>(buf);

                                    counts.push_back(count);

                                    if (run == 1) {
                                        std::copy_n(src, inputPart1.length(), std::back_inserter(result));
                                        return inputPart1.length();
                                    }
                                    if (run == 2) {
                                        std::copy_n(src, inputPart2.length(), std::back_inserter(result));
                                        return inputPart2.length();
                                    }
                                    else {
                                        throw std::logic_error("Write function shouldn't be called.");
                                    }
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, input);

        CHECK(result == inputPart1 + inputPart2);
        CHECK(counts.size() == 2);
        CHECK(counts.at(0) == input.length());
        CHECK(counts.at(1) == input.length() - inputPart1.length());
    }

    UNIT_TEST("Should ignore remaining input string when children closes stdin after poll (poll returns POLLOUT but SIGPIPE is received and system write returns EPIPE)")
    {
        const std::string inputPart1 = "Short";
        const std::string inputPart2 = "ProcessInput";
        const std::string input = inputPart1 + inputPart2;

        std::string result;
        std::vector<ssize_t> counts;
        bool isSigPipeIgnored = false;
        system::unix::WriteOptions receivedWriteOption;

        systemFake.Signal = [&](int signum, sighandler_t handler){
                                 if (signum == SIGPIPE && handler == SIG_IGN) {
                                     isSigPipeIgnored = true;
                                 }
                            };
        systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                    ++run;
                                    fds[0].revents = POLLHUP;
                                    if (run < 3) {
                                        fds[1].revents = POLLOUT;
                                    }
                                    else {
                                        fds[1].revents = POLLERR;
                                    }
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.WriteAction = [&, run = 0](int fd, const void *buf, size_t count, system::unix::WriteOptions writeOption) mutable -> ssize_t {
                                    ++run;
                                    auto src = static_cast<const char*>(buf);

                                    counts.push_back(count);

                                    if (run == 1) {
                                        std::copy_n(src, inputPart1.length(), std::back_inserter(result));
                                        return inputPart1.length();
                                    }
                                    if (run == 2) {
                                        /* when unix::Write receive EPIPE then it returns 0 */
                                        receivedWriteOption = writeOption;
                                        return 0;
                                    }
                                    else {
                                        throw std::logic_error("Write function shouldn't be called.");
                                    }
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, input);

        CHECK(isSigPipeIgnored == true);
        CHECK(result == inputPart1);
        CHECK(counts.size() == 2);
        CHECK(counts.at(0) == input.length());
        CHECK(counts.at(1) == input.length() - inputPart1.length());
        CHECK(receivedWriteOption == system::unix::WriteOptions::IGNORE_EPIPE);
    }
}


TEST_GROUP("Pipes Management")
{
    system::unix::ResetGlobalFake();

    constexpr int toParentReadEnd = 12;
    constexpr int toParentWriteEnd = 13;
    constexpr system::unix::Pipe toParentPipe = {toParentReadEnd, toParentWriteEnd};

    constexpr int toChildReadEnd = 22;
    constexpr int toChildWriteEnd = 23;
    constexpr system::unix::Pipe toChildPipe = {toChildReadEnd, toChildWriteEnd};

    constexpr int toParentInternalErrorsReadEnd = 32;
    constexpr int toParentInternalErrorsWriteEnd = 33;
    constexpr system::unix::Pipe toParentInternalErrorsPipe = {toParentInternalErrorsReadEnd, toParentInternalErrorsWriteEnd};

    systemFake.MakePipeAction = [run = 0](const system::unix::PipeOptions option) mutable -> system::unix::Pipe {
                                    ++run;
                                    if (run == 1) {
                                        return toParentPipe;
                                    }
                                    else if (run == 2) {
                                        return toChildPipe;
                                    }
                                    else if (run == 3) {
                                        return toParentInternalErrorsPipe;
                                    }
                                    else {
                                        throw std::logic_error("Unexpected call to system::unix::Pipe().");
                                    }
                                };
    systemFake.WaitPidAction = [](int pid, int *wstatus, int options) {
                                 return 1;
                               };
    systemFake.PollAction = [](struct pollfd *fds, nfds_t nfds, int timeout) {
                                    fds[0].revents = POLLHUP;
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
    systemFake.SigAction = [](int, const struct sigaction*, struct sigaction*) {};
    systemFake.Signal = [](int, sighandler_t){};
    systemFake.FcntlAction = [](int, int, int) { return 0; };


    SUBGROUP("Parent Process")
    {
        systemFake.ForkAction = []() { return anyChildProcessId; };
        systemFake.WriteAction = [](int, const void *, size_t, system::unix::WriteOptions) -> ssize_t { return 0; };
        systemFake.ReadAction = [](int, void *, size_t) -> ssize_t { return 0; };


        UNIT_TEST("Should only make to children write end as non blocking, children side should block like the standard input")
        {
            systemFake.CloseAction = [](int fd) {};
            systemFake.FcntlAction = [](int fd, int cmd, int arg) {
                                         if (cmd == F_SETFL) {
                                             if (fd != toChildWriteEnd && arg == O_NONBLOCK) {
                                                 throw std::logic_error("O_NONBLOCK F_SETFL on wrong fd:" + std::to_string(fd));
                                             }
                                         }
                                         else if (cmd == F_GETFL) {
                                             /* do nothing */
                                         }
                                         else {
                                             throw std::logic_error("Unknown command for fctl()");
                                         }
                                         return 0;
                                     };

            (void) RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);
        }

        UNIT_TEST("Should close all pipes ends during function lifetime")
        {
            bool isClosedToParentReadEnd = false;
            bool isClosedToParentWriteEnd = false;
            bool isClosedToChildReadEnd = false;
            bool isClosedToChildWriteEnd = false;
            bool isClosedToParentInternalErrorsReadEnd = false;
            bool isClosedToParentInternalErrorsWriteEnd = false;

            systemFake.CloseAction = [&](int fd) {
                                         if (isClosedToParentReadEnd == false && toParentReadEnd == fd) {
                                             isClosedToParentReadEnd = true;
                                         }
                                         else if (isClosedToParentWriteEnd == false && toParentWriteEnd == fd) {
                                             isClosedToParentWriteEnd = true;
                                         }
                                         else if (isClosedToChildReadEnd == false && toChildReadEnd == fd) {
                                             isClosedToChildReadEnd = true;
                                         }
                                         else if (isClosedToChildWriteEnd == false && toChildWriteEnd == fd) {
                                             isClosedToChildWriteEnd = true;
                                         }
                                         else if (isClosedToParentInternalErrorsReadEnd == false && toParentInternalErrorsReadEnd == fd) {
                                             isClosedToParentInternalErrorsReadEnd = true;
                                         }
                                         else if (isClosedToParentInternalErrorsWriteEnd == false && toParentInternalErrorsWriteEnd == fd) {
                                             isClosedToParentInternalErrorsWriteEnd = true;
                                         }
                                         else {
                                             throw std::logic_error("Unexpected fd to close or already closed: " + std::to_string(fd));
                                         }
                                     };

            (void) RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

            CHECK(isClosedToParentReadEnd == true);
            CHECK(isClosedToParentWriteEnd == true);
            CHECK(isClosedToChildReadEnd == true);
            CHECK(isClosedToChildWriteEnd == true);
            CHECK(isClosedToParentInternalErrorsReadEnd == true);
            CHECK(isClosedToParentInternalErrorsWriteEnd == true);
        }

        UNIT_TEST("Should close toChildWriteEnd after all the data was written to the pipe and the remote side is working")
        {
            int isClosedToChildWriteEnd = false;
            bool hupAllFds = false;

            systemFake.PollAction = [&,run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                        ++run;
                                        if (run == 1) {
                                            fds[0].revents = POLLIN;
                                            fds[1].revents = POLLOUT;
                                            fds[2].revents = POLLIN;
                                        }
                                        else if (run == 2) {
                                            fds[0].revents = POLLIN;
                                            fds[1].revents = POLLHUP;
                                            fds[2].revents = POLLIN;
                                        }
                                        else if (hupAllFds) {
                                            fds[0].revents = POLLHUP;
                                            fds[1].revents = POLLHUP;
                                            fds[2].revents = POLLHUP;
                                        }
                                        return 0;
                                    };
            systemFake.WriteAction = [&](int fd, const void *, size_t, system::unix::WriteOptions) -> ssize_t {
                                         if (fd == toChildWriteEnd) {
                                             isClosedToChildWriteEnd = false;
                                             return nonImportantNonEmptyInput.length();
                                         }
                                         else {
                                             throw std::logic_error("Unexpected call to system::unix::Write with fd: " + std::to_string(fd));
                                         }
                                     };
            systemFake.CloseAction = [&, call = 0](int fd) mutable {
                                         if (fd == toChildWriteEnd) {
                                             if (call == 0) {
                                                 isClosedToChildWriteEnd = true;
                                                 hupAllFds = true;
                                             }
                                             else {
                                                 throw std::logic_error("Double call of Close on this same fd: " + std::to_string(fd));
                                             }
                                         }
                                     };

            (void) RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantNonEmptyInput);

            CHECK(isClosedToChildWriteEnd == true);
        }

        UNIT_TEST("Should close toChildWriteEnd when all data is written to the pipe and then remote side hangs up")
        {
            int isClosedToChildWriteEnd = false;

            systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                        ++run;
                                        fds[0].revents = POLLHUP;
                                        constexpr int writeToChildRun = 1;
                                        if (run == writeToChildRun) {
                                            fds[1].revents = POLLOUT;
                                        }
                                        else {
                                            fds[1].revents = POLLHUP;
                                        }
                                        fds[2].revents = POLLHUP;
                                        return 0;
                                    };
            systemFake.WriteAction = [&](int fd, const void *, size_t, system::unix::WriteOptions) -> ssize_t {
                                         if (fd == toChildWriteEnd) {
                                             isClosedToChildWriteEnd = false;
                                             return nonImportantNonEmptyInput.length();
                                         }
                                         else {
                                             throw std::logic_error("Unexpected call to system::unix::Write with fd: " + std::to_string(fd));
                                         }
                                     };
            systemFake.CloseAction = [&, call = 0](int fd) mutable {
                                         if (fd == toChildWriteEnd) {
                                             if (call == 0) {
                                                 isClosedToChildWriteEnd = true;
                                             }
                                             else {
                                                 throw std::logic_error("Double call of Close on this same fd: " + std::to_string(fd));
                                             }
                                         }
                                     };

            (void) RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantNonEmptyInput);

            CHECK(isClosedToChildWriteEnd == true);
        }

        UNIT_TEST("Should close toChildWriteEnd when all data was not written to the pipe and then remote side hangs up")
        {
            const std::string input = "ShortProcessInput";
            int isClosedToChildWriteEnd = false;

            systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                        ++run;
                                        fds[0].revents = POLLHUP;
                                        constexpr int writeToChildRun = 1;
                                        if (run == writeToChildRun) {
                                            fds[1].revents = POLLOUT;
                                        }
                                        else {
                                            fds[1].revents = POLLHUP;
                                        }
                                        fds[2].revents = POLLHUP;
                                        return 0;
                                    };
            systemFake.WriteAction = [&](int fd, const void *, size_t, system::unix::WriteOptions) -> ssize_t {
                                         if (fd == toChildWriteEnd) {
                                             isClosedToChildWriteEnd = false;
                                             return 0;
                                         }
                                         else {
                                             throw std::logic_error("Unexpected call to system::unix::Write with fd: " + std::to_string(fd));
                                         }
                                     };
            systemFake.CloseAction = [&, call = 0](int fd) mutable {
                                         if (fd == toChildWriteEnd) {
                                             if (call == 0) {
                                                 isClosedToChildWriteEnd = true;
                                             }
                                             else {
                                                 throw std::logic_error("Double call of Close on this same fd: " + std::to_string(fd));
                                             }
                                         }
                                     };

            (void) RunProcess(exampleBinaryPath, emptyRunProcessArguments, input);

            CHECK(isClosedToChildWriteEnd == true);
        }

        UNIT_TEST("Should close toParentReadEnd after all the data was read from the pipe")
        {
            int isClosedToParentReadEnd = false;

            systemFake.ReadAction = [&](int fd, void *, size_t) -> ssize_t {
                                         if (fd == toParentReadEnd) {
                                             isClosedToParentReadEnd = false;
                                             return 0;
                                         }
                                         else if (fd == toParentInternalErrorsReadEnd) {
                                             return 0;
                                         }
                                         else {
                                             throw std::logic_error("Unexpected call to system::unix::Read with fd: " + std::to_string(fd));
                                         }
                                     };
            systemFake.CloseAction = [&, call = 0](int fd) mutable {
                                         if (fd == toParentReadEnd) {
                                             if (call == 0) {
                                                 isClosedToParentReadEnd = true;
                                                 ++call;
                                             }
                                             else {
                                                 throw std::logic_error("Double call of Close on this same fd: " + std::to_string(fd));
                                             }
                                         }
                                     };

            (void) RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

            CHECK(isClosedToParentReadEnd == true);
        }

        UNIT_TEST("Should close pipes ends before exception throw")
        {
            constexpr int readFromParentInternalErrorsReadEndRun = 1;
            bool isClosedToParentReadEnd = false;
            bool isClosedToParentInternalErrorsReadEnd = false;

            systemFake.WriteAction = [](int, const void *, size_t length, system::unix::WriteOptions) -> ssize_t {
                                         return length;
                                     };
            systemFake.ReadAction = [&, run = 0](int fd, void *buf, size_t) mutable -> ssize_t {
                                      ++run;
                                      auto dest = static_cast<char*>(buf);
                                      if (run == readFromParentInternalErrorsReadEndRun) {
                                          std::string someMessage = "some message";
                                          std::copy(someMessage.begin(), someMessage.end(), dest);
                                          return someMessage.length();
                                      }
                                      else {
                                          return 0;
                                      }
                                    };
            systemFake.CloseAction = [&](int fd) {
                                         if (isClosedToParentReadEnd == false && toParentReadEnd == fd) {
                                             isClosedToParentReadEnd = true;
                                         }
                                         else if (isClosedToParentInternalErrorsReadEnd == false && toParentInternalErrorsReadEnd == fd) {
                                             isClosedToParentInternalErrorsReadEnd = true;
                                         }
                                         else if (toParentReadEnd == fd
                                                  || toParentInternalErrorsReadEnd == fd) {
                                             throw std::logic_error("Unexpected fd to close or already closed: " + std::to_string(fd));
                                         }
                                     };
            systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                        ++run;
                                        fds[0].revents = POLLHUP;
                                        fds[1].revents = POLLHUP;
                                        if (run == readFromParentInternalErrorsReadEndRun) {
                                            fds[2].revents = POLLIN;
                                        }
                                        else {
                                            fds[2].revents = POLLHUP;
                                        }
                                        return 0;
                                    };

            CHECK_THROWS_AS(RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput),
                            omtt::exception::SutExecutionException);

            CHECK(isClosedToParentReadEnd == true);
            CHECK(isClosedToParentInternalErrorsReadEnd == true);
        }
    }

    SUBGROUP("Child Process")
    {
        systemFake.ForkAction = []() {
                                    constexpr int specialParentProcessId = 0;
                                    return specialParentProcessId;
                                };
        systemFake.CloseAction = [](int fd) {};
        systemFake.DuplicateFdAction = [](int oldFd, int newFd) {};
        systemFake.ExecAction = [](const std::string &, const std::vector<std::string> &) {};

        UNIT_TEST("Should close STDIN, STDOUT before duplicating fd")
        {
            bool isDuplicateFdExecutedForStdIn = false;
            bool isDuplicateFdExecutedForStdOut = false;
            bool isClosedStdIn = false;
            bool isClosedStdOut = false;

            systemFake.CloseAction = [&](int fd) {
                                         if (isDuplicateFdExecutedForStdIn == false
                                             && isClosedStdIn == false
                                             && fd == static_cast<int>(system::unix::FdId::STDIN)) {
                                             isClosedStdIn = true;
                                         }
                                         else if (isDuplicateFdExecutedForStdOut == false
                                             && isClosedStdOut == false
                                             && fd == static_cast<int>(system::unix::FdId::STDOUT)) {
                                             isClosedStdOut = true;
                                         }
                                         else if (fd == static_cast<int>(system::unix::FdId::STDOUT)
                                                  || fd == static_cast<int>(system::unix::FdId::STDIN)){
                                             throw std::logic_error("Wrong execution order or already closed fd: " + std::to_string(fd));
                                         }
                                     };
            systemFake.DuplicateFdAction = [&](int oldFd, int newFd) {
                                               if (newFd == static_cast<int>(system::unix::FdId::STDIN)) {
                                                   isDuplicateFdExecutedForStdIn = true;
                                               }
                                               else if (newFd == static_cast<int>(system::unix::FdId::STDOUT)) {
                                                   isDuplicateFdExecutedForStdOut = true;
                                               }
                                           };

            (void) RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

            CHECK(isClosedStdIn == true);
            CHECK(isClosedStdOut == true);
        }

        UNIT_TEST("Should close toParentReadEnd, toChildWriteEnd, toParentInternalErrorsReadEnd before Exec")
        {
            bool isExecExecuted = false;
            bool isClosedToParentReadEnd = false;
            bool isClosedToChildWriteEnd = false;
            bool isClosedToParentInternalErrorsReadEnd = false;

            systemFake.CloseAction = [&](int fd) {
                                         if (isExecExecuted == false) {
                                             if (isClosedToParentReadEnd == false && toParentReadEnd == fd) {
                                                 isClosedToParentReadEnd = true;
                                             }
                                             else if (isClosedToChildWriteEnd == false && toChildWriteEnd == fd) {
                                                 isClosedToChildWriteEnd = true;
                                             }
                                             else if (isClosedToParentInternalErrorsReadEnd == false && toParentInternalErrorsReadEnd == fd) {
                                                 isClosedToParentInternalErrorsReadEnd = true;
                                             }
                                             else if (toParentReadEnd == fd
                                                      || toChildWriteEnd == fd
                                                      || toParentInternalErrorsReadEnd == fd) {
                                                 throw std::logic_error("Already closed: " + std::to_string(fd));
                                             }
                                         }
                                         else {
                                             throw std::logic_error("Close should be executed before Exec function for fd: " + std::to_string(fd));
                                         }
                                     };
            systemFake.ExecAction = [&](const std::string &, const std::vector<std::string> &) {
                                        isExecExecuted = true;
                                    };

            (void) RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

            CHECK(isClosedToParentReadEnd == true);
            CHECK(isClosedToChildWriteEnd == true);
            CHECK(isClosedToParentInternalErrorsReadEnd == true);
        }

        UNIT_TEST("Should duplicate toChildReadEnd to STDIN")
        {
            bool isDuplicatedToChildReadEnd = false;

            systemFake.DuplicateFdAction = [&](int oldFd, int newFd) {
                                               if (oldFd == toChildReadEnd
                                                   && newFd == static_cast<int>(system::unix::FdId::STDIN)) {
                                                   isDuplicatedToChildReadEnd = true;
                                               }
                                           };

            (void) RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

            CHECK(isDuplicatedToChildReadEnd == true);
        }

        UNIT_TEST("Should duplicate toParentWriteEnd to STDOUT")
        {
            bool isDuplicatedToParentWriteEnd = false;

            systemFake.DuplicateFdAction = [&](int oldFd, int newFd) {
                                               if (oldFd == toParentWriteEnd
                                                   && newFd == static_cast<int>(system::unix::FdId::STDOUT)) {
                                                   isDuplicatedToParentWriteEnd = true;
                                               }
                                           };

            (void) RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

            CHECK(isDuplicatedToParentWriteEnd == true);
        }


        UNIT_TEST("Should write error message to parent's internal pipe and terminate when error occour during SUT process creation")
        {
            const std::string expectedErrorMessage = "some error message";
            std::string resultMessage;
            bool isTerminateCalled = false;

            systemFake.WriteAction = [&](int fd, const void *buf, size_t count, system::unix::WriteOptions) -> ssize_t {
                                         auto src = static_cast<const char*>(buf);
                                         std::string s(src, count);
                                         resultMessage += s;
                                         return s.length();
                                     };
            systemFake.TerminateAction = [&](const int exitCode) {
                                             if (exitCode == FATAL_ERROR) {
                                                 isTerminateCalled = true;
                                             }
                                             else {
                                                 throw std::logic_error("Terminate function not called.");
                                             }
                                         };

            SUBTEST("Error occour on toParentReadEnd pipe close")
            {
                systemFake.CloseAction = [&](int fd) {
                                             if (fd == toParentReadEnd) {
                                                 throw std::runtime_error(expectedErrorMessage);
                                             }
                                             else {
                                                 throw std::logic_error("Unexpected Close call with fd:" + std::to_string(fd));
                                             }
                                         };

                CHECK_THROWS_AS(RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput),
                                std::runtime_error);

                CHECK(resultMessage == expectedErrorMessage);
                CHECK(isTerminateCalled == true);
            }

            SUBTEST("Error occour on toChildWriteEnd pipe close")
            {
                systemFake.CloseAction = [&](int fd) {
                                             if (fd == toParentReadEnd) {
                                                 // do nothing
                                             }
                                             else if (fd == toChildWriteEnd) {
                                                 throw std::runtime_error(expectedErrorMessage);
                                             }
                                             else {
                                                 throw std::logic_error("Unexpected Close call with fd:" + std::to_string(fd));
                                             }
                                         };

                CHECK_THROWS_AS(RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput),
                                std::runtime_error);

                CHECK(resultMessage == expectedErrorMessage);
                CHECK(isTerminateCalled == true);
            }

            SUBTEST("Error occour on toParentInternalErrorsReadEnd pipe close")
            {
                systemFake.CloseAction = [&](int fd) {
                                             if (fd == toParentReadEnd
                                                 || fd == toChildWriteEnd) {
                                                 // do nothing
                                             }
                                             else if (fd == toParentInternalErrorsReadEnd) {
                                                 throw std::runtime_error(expectedErrorMessage);
                                             }
                                             else {
                                                 throw std::logic_error("Unexpected Close call with fd:" + std::to_string(fd));
                                             }
                                         };

                CHECK_THROWS_AS(RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput),
                                std::runtime_error);

                CHECK(resultMessage == expectedErrorMessage);
                CHECK(isTerminateCalled == true);
            }

            SUBTEST("Error occour on STDIN pipe close")
            {
                systemFake.CloseAction = [&](int fd) {
                                             if (fd == toParentReadEnd
                                                 || fd == toChildWriteEnd
                                                 || fd == toParentInternalErrorsReadEnd) {
                                                 // do nothing
                                             }
                                             else if (fd == static_cast<int>(system::unix::FdId::STDIN)) {
                                                 throw std::runtime_error(expectedErrorMessage);
                                             }
                                             else {
                                                 throw std::logic_error("Unexpected Close call with fd:" + std::to_string(fd));
                                             }
                                         };

                CHECK_THROWS_AS(RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput),
                                std::runtime_error);

                CHECK(resultMessage == expectedErrorMessage);
                CHECK(isTerminateCalled == true);
            }

            SUBTEST("Error occour on STDIN pipe duplication")
            {
                systemFake.CloseAction = [&](int fd) {
                                             if (fd == toParentReadEnd
                                                 || fd == toChildWriteEnd
                                                 || fd == toParentInternalErrorsReadEnd
                                                 || fd == static_cast<int>(system::unix::FdId::STDIN)) {
                                                 // do nothing
                                             }
                                             else {
                                                 throw std::logic_error("Unexpected Close call with fd:" + std::to_string(fd));
                                             }
                                         };
                systemFake.DuplicateFdAction = [&](int oldFd, int newFd) {
                                                   if (newFd == static_cast<int>(system::unix::FdId::STDIN)) {
                                                       throw std::runtime_error(expectedErrorMessage);
                                                   }
                                                   else {
                                                       throw std::logic_error("Unexpected DuplicateFd call with newFd:" + std::to_string(newFd));
                                                   }
                                               };

                CHECK_THROWS_AS(RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput),
                                std::runtime_error);

                CHECK(resultMessage == expectedErrorMessage);
                CHECK(isTerminateCalled == true);
            }

            SUBTEST("Error occour on STDOUT pipe close")
            {
                systemFake.CloseAction = [&](int fd) {
                                             if (fd == toParentReadEnd
                                                 || fd == toChildWriteEnd
                                                 || fd == toParentInternalErrorsReadEnd
                                                 || fd == static_cast<int>(system::unix::FdId::STDIN)) {
                                                 // do nothing
                                             }
                                             else if (fd == static_cast<int>(system::unix::FdId::STDOUT)) {
                                                 throw std::runtime_error(expectedErrorMessage);
                                             }
                                             else {
                                                 throw std::logic_error("Unexpected Close call with fd:" + std::to_string(fd));
                                             }
                                         };

                CHECK_THROWS_AS(RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput),
                                std::runtime_error);

                CHECK(resultMessage == expectedErrorMessage);
                CHECK(isTerminateCalled == true);
            }

            SUBTEST("Error occour on STDOUT pipe duplication")
            {
                systemFake.CloseAction = [&](int fd) {
                                             if (fd == toParentReadEnd
                                                 || fd == toChildWriteEnd
                                                 || fd == toParentInternalErrorsReadEnd
                                                 || fd == static_cast<int>(system::unix::FdId::STDIN)
                                                 || fd == static_cast<int>(system::unix::FdId::STDOUT)) {
                                                 // do nothing
                                             }
                                             else {
                                                 throw std::logic_error("Unexpected Close call with fd:" + std::to_string(fd));
                                             }
                                         };
                systemFake.DuplicateFdAction = [&](int oldFd, int newFd) {
                                                   if (newFd == static_cast<int>(system::unix::FdId::STDIN)) {
                                                       // do nothing
                                                   }
                                                   else if (newFd == static_cast<int>(system::unix::FdId::STDOUT)) {
                                                       throw std::runtime_error(expectedErrorMessage);
                                                   }
                                                   else {
                                                       throw std::logic_error("Unexpected DuplicateFd call with newFd:" + std::to_string(newFd));
                                                   }
                                               };

                CHECK_THROWS_AS(RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput),
                                std::runtime_error);

                CHECK(resultMessage == expectedErrorMessage);
                CHECK(isTerminateCalled == true);
            }
        }
    }
}

TEST_GROUP("SUT Process Execution")
{
    system::unix::ResetGlobalFake();

    systemFake.MakePipeAction = [](const system::unix::PipeOptions option) -> system::unix::Pipe { return {0,0}; };
    systemFake.WaitPidAction = [](int pid, int *wstatus, int options) { return 1; };
    systemFake.CloseAction = [](int fd) {};
    systemFake.DuplicateFdAction = [](int oldFd, int newFd) {};
    systemFake.SigAction = [](int, const struct sigaction*, struct sigaction*) {};
    systemFake.Signal = [](int, sighandler_t){};
    systemFake.FcntlAction = [](int, int, int) { return 0; };


    SUBGROUP("Parent side")
    {
        systemFake.ForkAction = []() { return anyChildProcessId; };

        UNIT_TEST("Should throw exception with error message from child process")
        {
            constexpr int readFromParentInternalErrorsReadEndRun = 1;
            std::string errorMessage = "message text";
            std::string expectedErrorMessage = "during SUT execution: " + errorMessage;

            systemFake.WriteAction = [](int, const void *, size_t length, system::unix::WriteOptions) -> ssize_t {
                                         return length;
                                     };
            systemFake.ReadAction = [&, run = 0](int fd, void *buf, size_t) mutable -> ssize_t {
                                      ++run;
                                      auto dest = static_cast<char*>(buf);
                                      if (run == readFromParentInternalErrorsReadEndRun) {
                                          std::copy(errorMessage.begin(), errorMessage.end(), dest);
                                          return errorMessage.length();
                                      }
                                      else {
                                          return 0;
                                      }
                                    };
            systemFake.CloseAction = [](int fd) {};
            systemFake.PollAction = [run = 0](struct pollfd *fds, nfds_t nfds, int timeout) mutable {
                                        ++run;
                                        fds[0].revents = POLLHUP;
                                        fds[1].revents = POLLHUP;
                                        if (run == readFromParentInternalErrorsReadEndRun) {
                                            fds[2].revents = POLLIN;
                                        }
                                        else {
                                            fds[2].revents = POLLHUP;
                                        }
                                        return 0;
                                    };

            CHECK_THROWS_WITH_AS(RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput),
                                 expectedErrorMessage.c_str(),
                                 omtt::exception::SutExecutionException);
        }
    }

    SUBGROUP("Child side")
    {
        systemFake.ForkAction = []() {
                                    constexpr int specialParentProcessId = 0;
                                    return specialParentProcessId;
                                };

        UNIT_TEST("Should execute specified binary with arguments")
        {
            std::string receivedPath;
            std::vector<std::string> receivedArguments;
            std::vector<std::string> expectedArguments = {"some argument"};

            systemFake.ExecAction = [&](const std::string &path, const std::vector<std::string> &arguments) {
                                        receivedPath = path;
                                        receivedArguments = arguments;
                                    };

            (void) RunProcess(exampleBinaryPath, expectedArguments, nonImportantEmptyInput);

            CHECK(receivedPath == exampleBinaryPath);
            CHECK(receivedArguments == expectedArguments);
        }

        UNIT_TEST("Should write error message to parent's internal pipe and terminate when error occour during SUT process exec")
        {
            const std::string expectedErrorMessage = "some error message";
            std::string resultMessage;
            bool isTerminateCalled = false;

            systemFake.ExecAction = [&](const std::string &path, const std::vector<std::string> &arguments) {
                                        throw std::runtime_error(expectedErrorMessage);
                                    };
            systemFake.WriteAction = [&](int fd, const void *buf, size_t count, system::unix::WriteOptions) -> ssize_t {
                                         auto src = static_cast<const char*>(buf);
                                         std::string s(src, count);
                                         resultMessage += s;
                                         return s.length();
                                     };
            systemFake.TerminateAction = [&](const int exitCode) {
                                             if (exitCode == FATAL_ERROR) {
                                                 isTerminateCalled = true;
                                             }
                                             else {
                                                 throw std::logic_error("Terminate function not called.");
                                             }
                                         };

            CHECK_THROWS_AS(RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput),
                            std::runtime_error);

            CHECK(resultMessage == expectedErrorMessage);
            CHECK(isTerminateCalled == true);
        }
    }
}


TEST_GROUP("Signals Management")
{
    system::unix::ResetGlobalFake();

    systemFake.MakePipeAction = [](const system::unix::PipeOptions option) -> system::unix::Pipe { return {0, 0}; };
    systemFake.ForkAction = []() { return anyChildProcessId; };
    systemFake.CloseAction = [](int) {};
    systemFake.WriteAction = [](int, const void *, size_t, system::unix::WriteOptions) -> ssize_t { return 0; };
    systemFake.ReadAction = [](int fd, void *buf, size_t count) -> ssize_t { return 0; };
    systemFake.WaitPidAction = [](int pid, int *wstatus, int options) {
                                 return 1;
                               };
    systemFake.FcntlAction = [](int, int, int) { return 0; };
    systemFake.SigAction = [&](int signum, const struct sigaction*, struct sigaction*) {};
    systemFake.Signal = [&](int signum, sighandler_t handler) {};


    UNIT_TEST("Should attach signals and reset them to defaults after process execution")
    {
        std::vector<int> expectedSignals = {SIGHUP, SIGINT, SIGTERM, SIGUSR1, SIGUSR2},
                         attachedSignals,
                         resetedSignals;
        systemFake.PollAction = [](struct pollfd *fds, nfds_t nfds, int timeout) {
                                    fds[0].revents = POLLHUP;
                                    fds[1].revents = POLLHUP;
                                    fds[2].revents = POLLHUP;
                                    return 0;
                                };
        systemFake.SigAction = [&](int signum, const struct sigaction*, struct sigaction*) {
                                   attachedSignals.push_back(signum);
                               };
        systemFake.Signal = [&](int signum, sighandler_t handler) {
                                if (handler == SIG_DFL) {
                                    resetedSignals.push_back(signum);
                                }
                                else if (handler == SIG_IGN) {
                                    /* ignore */
                                }
                                else {
                                    throw std::logic_error("Unexpected handler in signal() call for " + std::to_string(signum) + " signal.");
                                }
                            };

        ProcessResults results = RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput);

        CHECK(attachedSignals == expectedSignals);
        CHECK(resetedSignals == expectedSignals);
    }

    auto RunParameterizedTestForTerminateChildrenProcessAndThrowException = [&](const int signum)
    {
        bool isChildProcessTerminated = false;

        systemFake.PollAction = [](struct pollfd *fds, nfds_t nfds, int timeout) {
                                    fds[0].revents = POLLIN;
                                    fds[1].revents = POLLOUT;
                                    fds[2].revents = POLLIN;
                                    return 0;
                                };
        systemFake.ReadAction = [&](int fd, void *buf, size_t count) -> ssize_t {
                                    RunProcessSignalHandler(signum, nullptr, nullptr);
                                    return 0;
                                 };
        systemFake.ForkAction = []() {
                                    return anyChildProcessId;
                                };
        systemFake.KillAction = [&](pid_t pid, int sig) {
                                    if (pid ==  anyChildProcessId) {
                                        isChildProcessTerminated = true;
                                    }
                                };

        CHECK_THROWS_AS(RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput),
                        omtt::exception::SignalReceivedException);

        CHECK(isChildProcessTerminated == true);
    };

    UNIT_TEST("Should terminate working children process when SIGHUP is received and throw SignalReceivedException")
    {
        RunParameterizedTestForTerminateChildrenProcessAndThrowException(SIGHUP);
    }

    UNIT_TEST("Should terminate working children process when SIGINT is received and throw SignalReceivedException")
    {
        RunParameterizedTestForTerminateChildrenProcessAndThrowException(SIGINT);
    }

    UNIT_TEST("Should terminate working children process when SIGTERM is received and throw SignalReceivedException")
    {
        RunParameterizedTestForTerminateChildrenProcessAndThrowException(SIGTERM);
    }

    UNIT_TEST("Should terminate working children process when SIGUSR1 is received and throw SignalReceivedException")
    {
        RunParameterizedTestForTerminateChildrenProcessAndThrowException(SIGUSR1);
    }

    UNIT_TEST("Should terminate working children process when SIGUSR2 is received and throw SignalReceivedException")
    {
        RunParameterizedTestForTerminateChildrenProcessAndThrowException(SIGUSR2);
    }

    UNIT_TEST("Should pass system exception when children termination fails after any signal is received")
    {
        systemFake.PollAction = [](struct pollfd *fds, nfds_t nfds, int timeout) {
                                    fds[0].revents = POLLIN;
                                    fds[1].revents = POLLOUT;
                                    fds[2].revents = POLLIN;
                                    return 0;
                                };
        systemFake.ReadAction = [&](int fd, void *buf, size_t count) -> ssize_t {
                                    constexpr auto anySignal = SIGTERM;
                                    RunProcessSignalHandler(anySignal, nullptr, nullptr);
                                    return 0;
                                 };
        systemFake.ForkAction = []() {
                                    return anyChildProcessId;
                                };
        systemFake.KillAction = [&](pid_t pid, int sig) {
                                    constexpr auto someErrno = 1;
                                    throw omtt::system::unix::exception::SystemException("kill exception", someErrno);
                                };

        CHECK_THROWS_AS(RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput),
                        omtt::system::unix::exception::SystemException);
    }
}


TEST_GROUP("Unit tests dependencies")
{
    systemFake.WriteAction = [&](int fd, const void *buf, size_t count, system::unix::WriteOptions) -> ssize_t {
                               auto src = static_cast<const char*>(buf);
                               std::string s(src, count);
                               return s.length();
                             };
    systemFake.TerminateAction = [&](const int exitCode) {};
    systemFake.SigAction = [](int, const struct sigaction*, struct sigaction*) {};
    systemFake.Signal = [](int, sighandler_t){};
    systemFake.FcntlAction = [](int, int, int) { return 0; };


    UNIT_TEST("Should rethrow exceptions to allow error detecting in some unittests (catch could match the exception raised in ut)")
    {
        systemFake.CloseAction = [&](int fd) {
                                   throw std::runtime_error("any message");
                                 };

        CHECK_THROWS_AS(RunProcess(exampleBinaryPath, emptyRunProcessArguments, nonImportantEmptyInput),
                        std::runtime_error);
    }
}

}  // omtt
