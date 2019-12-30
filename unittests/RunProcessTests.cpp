/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/RunProcess.hpp"
#include "unittests/system/UnixFake.hpp"

#include "unittests/test_framework.hpp"

#include <iterator>
#include <string>


namespace omtt
{

auto &systemFake = system::unix::GlobalFake();

constexpr const char *exampleBinaryPath = "/bin/example";
constexpr int anyChildProcessId = 1347;

const std::string nonImportantEmptyInput;
const std::string nonImportantNonEmptyInput = "XYZ";


TEST_GROUP("Child Process Exit Code")
{
    system::unix::ResetGlobalFake();

    systemFake.MakePipeAction = []() -> system::unix::Pipe { return {0, 0}; };
    systemFake.ForkAction = []() { return anyChildProcessId; };
    systemFake.CloseAction = [](int) {};
    systemFake.WriteAction = [](int, const void *, size_t) -> ssize_t { return 0; };
    systemFake.ReadAction = [](int fd, void *buf, size_t count) -> ssize_t { return 0; };


    UNIT_TEST("Should return correct exit code")
    {
        int expectedExitCode = 1423;

        systemFake.ExitStatusAction = [&](int) {
                                          return expectedExitCode;
                                      };

        ProcessResults results = RunProcess(exampleBinaryPath, nonImportantEmptyInput);

        CHECK(results.exitCode == expectedExitCode);
    }
}


TEST_GROUP("Read Child Process Output")
{
    system::unix::ResetGlobalFake();

    systemFake.MakePipeAction = []() -> system::unix::Pipe { return {0, 0}; };
    systemFake.ForkAction = []() { return anyChildProcessId; };
    systemFake.CloseAction = [](int) {};
    systemFake.WriteAction = [](int, const void *, size_t) -> ssize_t { return 0; };
    systemFake.ExitStatusAction = [](int) { return 0; };


    UNIT_TEST("Should return empty output when child output is empty")
    {
        systemFake.ReadAction = [](int fd, void *buf, size_t count) -> ssize_t {
                                    return 0;
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, nonImportantEmptyInput);

        CHECK(results.output == "");
    }

    UNIT_TEST("Should return process output when output is short")
    {
        const std::string expectedProcessOutput = "ShortProcessOutput";

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

        ProcessResults results = RunProcess(exampleBinaryPath, nonImportantEmptyInput);

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

        ProcessResults results = RunProcess(exampleBinaryPath, nonImportantEmptyInput);

        CHECK(results.output == expectedProcessOutput);
    }
}


TEST_GROUP("Write To Child Process")
{
    system::unix::ResetGlobalFake();

    systemFake.MakePipeAction = []() -> system::unix::Pipe { return {0, 0}; };
    systemFake.ForkAction = []() { return anyChildProcessId; };
    systemFake.CloseAction = [](int) {};
    systemFake.ReadAction = [](int, const void *, size_t) -> ssize_t { return 0; };
    systemFake.ExitStatusAction = [](int) { return 0; };


    UNIT_TEST("Should not pass input when empty input string is given")
    {
        systemFake.WriteAction = [&](int fd, const void *buf, size_t count) -> ssize_t {
                                     throw std::logic_error("Write function shouldn't be called.");
                                 };

        ProcessResults results = RunProcess(exampleBinaryPath, "");
    }

    UNIT_TEST("Should pass input when input string is short")
    {
        const std::string input = "ShortProcessInput";
        std::string result;
        std::vector<ssize_t> counts;

        systemFake.WriteAction = [&](int fd, const void *buf, size_t count) -> ssize_t {
                                    auto src = static_cast<const char*>(buf);
                                    std::string s(src, count);
                                    counts.push_back(count);
                                    result += s;
                                    return s.length();
                                };

        ProcessResults results = RunProcess(exampleBinaryPath, input);

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

        systemFake.WriteAction = [&, run = 0](int fd, const void *buf, size_t count) mutable -> ssize_t {
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

        ProcessResults results = RunProcess(exampleBinaryPath, input);

        CHECK(result == input);
        CHECK(counts.size() == 3);
        CHECK(counts.at(0) == input.length());
        CHECK(counts.at(1) == input.length() - sizeOfPart1);
        CHECK(counts.at(2) == input.length() - sizeOfPart1 - sizeOfPart2);
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

    systemFake.MakePipeAction = [run = 0]() mutable -> system::unix::Pipe {
                                    ++run;
                                    if (run == 1) {
                                        return toParentPipe;
                                    }
                                    else if (run == 2) {
                                        return toChildPipe;
                                    }
                                    else {
                                        throw std::logic_error("Unexpected call to system::unix::Pipe().");
                                    }
                                };
    systemFake.ExitStatusAction = [](int) { return 0; };


    SUBGROUP("Parent Process")
    {
        systemFake.ForkAction = []() { return anyChildProcessId; };
        systemFake.WriteAction = [](int, const void *, size_t) -> ssize_t { return 0; };
        systemFake.ReadAction = [](int, void *, size_t) -> ssize_t { return 0; };


        UNIT_TEST("Should close all pipes ends during function lifetime")
        {
            bool isClosedToParentReadEnd = false;
            bool isClosedToParentWriteEnd = false;
            bool isClosedToChildReadEnd = false;
            bool isClosedToChildWriteEnd = false;

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
                                         else {
                                             throw std::logic_error("Unexpected fd to close or already closed: " + std::to_string(fd));
                                         }
                                     };

            (void) RunProcess(exampleBinaryPath, nonImportantEmptyInput);

            CHECK(isClosedToParentReadEnd == true);
            CHECK(isClosedToParentWriteEnd == true);
            CHECK(isClosedToChildReadEnd == true);
            CHECK(isClosedToChildWriteEnd == true);
        }

        UNIT_TEST("Should close toChildWriteEnd after the data was written to the pipe")
        {
            int isClosedToChildWriteEnd = false;

            systemFake.WriteAction = [&](int fd, const void *, size_t) -> ssize_t {
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

            (void) RunProcess(exampleBinaryPath, nonImportantNonEmptyInput);

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

            (void) RunProcess(exampleBinaryPath, nonImportantEmptyInput);

            CHECK(isClosedToParentReadEnd == true);
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
        systemFake.ExecAction = [](const std::string &) {};


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

            (void) RunProcess(exampleBinaryPath, nonImportantEmptyInput);

            CHECK(isClosedStdIn == true);
            CHECK(isClosedStdOut == true);
        }

        UNIT_TEST("Should close toParentReadEnd, toChildWriteEnd before Exec")
        {
            bool isExecExecuted = false;
            bool isClosedToParentReadEnd = false;
            bool isClosedToChildWriteEnd = false;

            systemFake.CloseAction = [&](int fd) {
                                         if (isExecExecuted == false) {
                                             if (isClosedToParentReadEnd == false && toParentReadEnd == fd) {
                                                 isClosedToParentReadEnd = true;
                                             }
                                             else if (isClosedToChildWriteEnd == false && toChildWriteEnd == fd) {
                                                 isClosedToChildWriteEnd = true;
                                             }
                                             else if (toParentReadEnd == fd || toChildWriteEnd == fd) {
                                                 throw std::logic_error("Already closed: " + std::to_string(fd));
                                             }
                                         }
                                         else {
                                             throw std::logic_error("Close should be executed before Exec function for fd: " + std::to_string(fd));
                                         }
                                     };
            systemFake.ExecAction = [&](const std::string &) {
                                        isExecExecuted = true;
                                    };

            (void) RunProcess(exampleBinaryPath, nonImportantEmptyInput);

            CHECK(isClosedToParentReadEnd == true);
            CHECK(isClosedToChildWriteEnd == true);
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

            (void) RunProcess(exampleBinaryPath, nonImportantEmptyInput);

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

            (void) RunProcess(exampleBinaryPath, nonImportantEmptyInput);

            CHECK(isDuplicatedToParentWriteEnd == true);
        }
    }
}

TEST_GROUP("SUT Process Execution")
{
    system::unix::ResetGlobalFake();

    systemFake.MakePipeAction = []() -> system::unix::Pipe { return {0,0}; };
    systemFake.ExitStatusAction = [](int) { return 0; };
    systemFake.ForkAction = []() {
                                constexpr int specialParentProcessId = 0;
                                return specialParentProcessId;
                            };
    systemFake.CloseAction = [](int fd) {};
    systemFake.DuplicateFdAction = [](int oldFd, int newFd) {};


    UNIT_TEST("Should execute specified binary")
    {
        std::string receivedPath;

        systemFake.ExecAction = [&](const std::string &path) {
                                    receivedPath = path;
                                };

        (void) RunProcess(exampleBinaryPath, nonImportantEmptyInput);

        CHECK(receivedPath == exampleBinaryPath);
    }
}

}  // omtt