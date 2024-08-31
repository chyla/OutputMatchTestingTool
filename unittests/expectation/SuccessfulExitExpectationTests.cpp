/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "unittests/test_framework.hpp"

#include "headers/expectation/SuccessfulExitExpectation.hpp"
#include "headers/ProcessResults.hpp"


namespace omtt::expectation
{

TEST_CASE("Should be satisfied when process exit code is zero")
{
    constexpr int processExitCode = 0;

    const ProcessResults sutResults {processExitCode};
    SuccessfulExitExpectation expectation;

    auto validationResults = expectation.Validate(sutResults);

    CHECK(validationResults.isSatisfied() == true);
}

TEST_CASE("Should not to be satisfied when process exit code is other than zero")
{
    constexpr int processExitCode = 1;

    const ProcessResults sutResults {processExitCode};
    SuccessfulExitExpectation expectation;

    auto validationResults = expectation.Validate(sutResults);

    CHECK(validationResults.isSatisfied() == false);
}

TEST_CASE("Should not contain cause when expectation is satisfied")
{
    constexpr int processExitCode = 0;

    const ProcessResults sutResults {processExitCode};
    SuccessfulExitExpectation expectation;

    auto validationResults = expectation.Validate(sutResults);

    CHECK(validationResults.cause.has_value() == false);
}

TEST_CASE("Should contain cause with process exit code when expectation is not satisfied")
{
    constexpr int processExitCode = 4;

    const ProcessResults sutResults {processExitCode};
    SuccessfulExitExpectation expectation;

    auto validationResults = expectation.Validate(sutResults);

    CHECK(validationResults.cause.has_value() == true);
    const auto cause = std::get<expectation::validation::SuccessfulExitCause>(*validationResults.cause);
    CHECK(cause.fExitCode == processExitCode);
}

}
