/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "unittests/test_framework.hpp"

#include "headers/expectation/FailureExitExpectation.hpp"
#include "headers/ProcessResults.hpp"


namespace omtt::expectation
{

TEST_CASE("Should be satisfied when process exit code is one")
{
    constexpr int processExitCode = 1;

    const ProcessResults sutResults {processExitCode};
    FailureExitExpectation expectation;

    auto validationResults = expectation.Validate(sutResults);

    CHECK(validationResults.isSatisfied() == true);
}

TEST_CASE("Should be satisfied when process exit code is grater than one")
{
    constexpr int processExitCode = 4;

    const ProcessResults sutResults {processExitCode};
    FailureExitExpectation expectation;

    auto validationResults = expectation.Validate(sutResults);

    CHECK(validationResults.isSatisfied() == true);
}

TEST_CASE("Should be satisfied when process exit code is less than zero")
{
    constexpr int processExitCode = -1;

    const ProcessResults sutResults {processExitCode};
    FailureExitExpectation expectation;

    auto validationResults = expectation.Validate(sutResults);

    CHECK(validationResults.isSatisfied() == true);
}

TEST_CASE("Should not to be satisfied when process exit code is zero")
{
    constexpr int processExitCode = 0;

    const ProcessResults sutResults {processExitCode};
    FailureExitExpectation expectation;

    auto validationResults = expectation.Validate(sutResults);

    CHECK(validationResults.isSatisfied() == false);
}

TEST_CASE("Should not contain cause when expectation is satisfied")
{
    constexpr int processExitCode = 1;

    const ProcessResults sutResults {processExitCode};
    FailureExitExpectation expectation;

    auto validationResults = expectation.Validate(sutResults);

    CHECK(validationResults.cause.has_value() == false);
}

TEST_CASE("Should contain cause with process exit code when expectation is not satisfied")
{
    constexpr int processExitCode = 0;

    const ProcessResults sutResults {processExitCode};
    FailureExitExpectation expectation;

    auto validationResults = expectation.Validate(sutResults);

    CHECK(validationResults.cause.has_value() == true);
    const auto cause = std::get<expectation::validation::FailureExitCause>(*validationResults.cause);
    CHECK(cause.fExitCode == processExitCode);
}

}
