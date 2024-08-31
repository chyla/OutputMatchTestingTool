/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "headers/logger/ConsoleLogger.hpp"
#include "headers/logger/detail/Context.hpp"

#include <iostream>


namespace omtt::logger
{

void
ConsoleLogger::SutPath(const std::string &path)
{
    stream << "Testing: " << path << '\n';
}

void
ConsoleLogger::BeginTestExecution(const omtt::TestPaths::size_type executedTests,
                                  const omtt::TestPaths::size_type numberOfTests,
                                  const omtt::Path &testPath)
{
    stream << "====================\n"
              "Running test (" << executedTests << "/" << numberOfTests << "): " << testPath << '\n';
}

namespace
{

struct CauseVisitor {
    CauseVisitor(std::ostream &stream) : stream(stream) {}

    void operator()(expectation::validation::EmptyOutputCause cause) {
        constexpr int differencePosition = 0;

        stream << "Expected empty output.\n"
                  "Got (context):\n"
                  + detail::context(cause.fOutput,
                                    differencePosition,
                                    detail::PointerVisibility::NO_POINTER);
    }

    void operator()(expectation::validation::ExitCodeCause cause) {
        stream << "Exit code doesn't match.\n"
                  "Expected: " + std::to_string(cause.fExpectedExitCode) + "\n"
                  "Got: " + std::to_string(cause.fExitCode);
    }

    void operator()(expectation::validation::FullOutputCause cause) {
        stream << "Output doesn't match.\n"
                  "First difference at byte: " + std::to_string(cause.fDifferencePosition) + "\n"
                  + "Expected (context):\n" + detail::context(cause.fExpectedOutput,
                                                            cause.fDifferencePosition,
                                                            detail::PointerVisibility::INCLUDE_POINTER) + "\n"
                  + "Got (context):\n" + detail::context(cause.fOutput,
                                                          cause.fDifferencePosition,
                                                          detail::PointerVisibility::INCLUDE_POINTER);
    }

    void operator()(expectation::validation::PartialOutputCause cause) {
        constexpr int differencePosition = 0;

        stream << "Text not found in output.\n"
                  "Expected (context):\n"
                  + detail::context(cause.fExpectedPartialOutput,
                                    differencePosition,
                                    detail::PointerVisibility::NO_POINTER);
    }

    void operator()(expectation::validation::SuccessfulExitCause cause) {
        stream << "Exit status doesn't match.\n"
                  "Expected: exit with success\n"
                  "Got: exit with failure (exit code: " + std::to_string(cause.fExitCode) + ")";
    }


    void operator()(expectation::validation::FailureExitCause cause) {
        stream << "Exit status doesn't match.\n"
                  "Expected: exit with failure (exit code other than zero)\n"
                  "Got: exit with success (exit code: " + std::to_string(cause.fExitCode) + ")";
    }

private:
    std::ostream &stream;
};

}

void
ConsoleLogger::EndTestExecution(const omtt::ProcessResults &processResults,
                                const omtt::TestExecutionSummary &summary)
{
    stream << "Verdict: " << to_cstring(summary.verdict);

    CauseVisitor visitor(stream);
    for (const auto &cause : summary.causes) {
        stream << "\n"
                  "--------------------\n"
                  "=> Cause:\n";
        std::visit(visitor, cause);
    }

    stream << '\n';

    if (processResults.errors.length() > 0) {
        stream << "--------------------\n"
                  "=> SUT error messages printed during test execution:\n"
               << processResults.errors;

        if (*processResults.errors.rbegin() != '\n') {
            stream << '\n';
        }
    }
}

void
ConsoleLogger::OverallStatistics(const omtt::TestPaths::size_type executedTests,
                                 const omtt::TestPaths::size_type numberOfTestsPassed,
                                 const omtt::TestPaths::size_type numberOfTestsFailed)
{
    stream << "====================\n"
           << executedTests << " tests total, " << numberOfTestsPassed << " passed, " << numberOfTestsFailed << " failed\n";
}

}
