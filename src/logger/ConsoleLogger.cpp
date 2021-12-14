/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
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

private:
    std::ostream &stream;
};

}

void
ConsoleLogger::EndTestExecution(const omtt::TestExecutionSummary &summary)
{
    stream << "Verdict: " <<  to_cstring(summary.verdict);

    CauseVisitor visitor(stream);
    for (const auto &cause : summary.causes) {
        stream << "\n"
                  "--------------------\n"
                  "=> Cause:\n";
        std::visit(visitor, cause);
    }

    stream << '\n';
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
