/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "unittests/test_framework.hpp"

#include "headers/expectation/ExitCodeExpectation.hpp"
#include "headers/ProcessResults.hpp"


namespace omtt::expectation
{

TEST_CASE("Should be satisfied when expected exit code is equal to process exit code")
{
    constexpr int expectedExitCode = 3;
    constexpr int processExitCode = 3;

    const ProcessResults sutResults {processExitCode, "some not important output"};
    ExitCodeExpectation expectation(expectedExitCode);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == true);
}

TEST_CASE("Should not to be satisfied when expected exit code is different to process exit code")
{
    constexpr int expectedExitCode = 3;
    constexpr int processExitCode = 4;

    const ProcessResults sutResults {processExitCode, "some not important output"};
    ExitCodeExpectation expectation(expectedExitCode);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
}


TEST_CASE("Should not contain cause when expectation is satisfied")
{
    constexpr int expectedExitCode = 3;
    constexpr int processExitCode = 3;

    const ProcessResults sutResults {processExitCode, "some not important output"};
    ExitCodeExpectation expectation(expectedExitCode);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == true);
    CHECK(validationReults.cause.has_value() == false);
}

TEST_CASE("Should contain cause with expected exit code when expectation is not satisfied")
{
    constexpr int expectedExitCode = 2;
    constexpr int processExitCode = 6;

    const ProcessResults sutResults {processExitCode, "some not important output"};
    ExitCodeExpectation expectation(expectedExitCode);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
    CHECK(validationReults.cause.has_value() == true);
    const auto cause = std::get<expectation::validation::ExitCodeCause>(*validationReults.cause);
    CHECK(cause.fExpectedExitCode == expectedExitCode);
}

TEST_CASE("Should contain cause with process exit code when expectation is not satisfied")
{
    constexpr int expectedExitCode = 2;
    constexpr int processExitCode = 6;

    const ProcessResults sutResults {processExitCode, "some not important output"};
    ExitCodeExpectation expectation(expectedExitCode);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
    CHECK(validationReults.cause.has_value() == true);
    const auto cause = std::get<expectation::validation::ExitCodeCause>(*validationReults.cause);
    CHECK(cause.fExitCode == processExitCode);
}

}
