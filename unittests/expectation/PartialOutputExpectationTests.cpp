/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "unittests/test_framework.hpp"

#include "headers/expectation/PartialOutputExpectation.hpp"
#include "headers/ProcessResults.hpp"


namespace omtt
{

TEST_CASE("Should be satisfied when SUT output contains given text")
{
    const std::string expectedPartialOutput = "me ou";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied() == true);
}

TEST_CASE("Should be satisfied on empty texts")
{
    const std::string expectedPartialOutput = "";
    const ProcessResults sutResults {0, ""};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied() == true);
}

TEST_CASE("Should be satisfied when expected partial output is empty text")
{
    const std::string expectedPartialOutput = "";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied() == true);
}

TEST_CASE("Should not be satisfied when SUT output doesn't contain given text")
{
    const std::string expectedPartialOutput = "other text";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied() == false);
}

TEST_CASE("Should not be satisfied when SUT output and given text differs in letters case")
{
    const std::string expectedPartialOutput = "somE output";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied() == false);
}

TEST_CASE("Cause should not be set when expectation match")
{
    const std::string expectedPartialOutput = "me ou";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied() == true);
    CHECK(!validationReults.cause.has_value());
}

TEST_CASE("Cause should be set when expectation didn't match")
{
    const std::string expectedPartialOutput = "other text";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied() == false);
    CHECK(validationReults.cause.has_value());
}

TEST_CASE("Cause should contain expected text")
{
    const std::string expectedPartialOutput = "other text";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied() == false);
    CHECK(validationReults.cause.has_value());
    const auto cause = std::get<expectation::validation::PartialOutputCause>(*validationReults.cause);
    CHECK(cause.fExpectedPartialOutput == expectedPartialOutput);
}

}
