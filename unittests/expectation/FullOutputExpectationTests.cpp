/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "unittests/test_framework.hpp"

#include "headers/expectation/FullOutputExpectation.hpp"
#include "headers/ProcessResults.hpp"


namespace omtt
{

TEST_CASE("Should be satisfied when expected output and SUT output is the same")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "some output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == true);
}

TEST_CASE("Should not be satisfied when expected output and SUT output is not the same")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "some other output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
}

TEST_CASE("Should not be satisfied when expected output and SUT output differs in letters case")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "Some Output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
}

TEST_CASE("Cause should not be set when expectation match")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "some output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == true);
    CHECK(!validationReults.cause.has_value());
}

TEST_CASE("Cause should be set when expectation didn't match")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "SOME OUTPUT"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
    CHECK(validationReults.cause.has_value());
}

TEST_CASE("Cause should not be set on empty texts")
{
    const std::string expectedOutput = "";
    const ProcessResults sutResults {0, ""};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == true);
    CHECK(!validationReults.cause.has_value());
}

TEST_CASE("Cause should point to first byte when the difference is on the first position")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "Some Output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(*validationReults.cause == "Output doesn't match.\nFirst difference at byte: 0");
}

TEST_CASE("Cause should point to fourth byte when the difference is on the fourth position")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "somE output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(*validationReults.cause == "Output doesn't match.\nFirst difference at byte: 3");
}

TEST_CASE("Cause should point to last byte when the difference is on the last position")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "some outpuT"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(*validationReults.cause == "Output doesn't match.\nFirst difference at byte: 10");
}

TEST_CASE("Cause should point after last byte of expectation text when expectation text is shorten than SUT output")
{
    const std::string expectedOutput = "some";
    const ProcessResults sutResults {0, "some text"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(*validationReults.cause == "Output doesn't match.\nFirst difference at byte: 4");
}

TEST_CASE("Cause should point after last byte of SUT output text when SUT output text is shorten than expectation text")
{
    const std::string expectedOutput = "some text";
    const ProcessResults sutResults {0, "some"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(*validationReults.cause == "Output doesn't match.\nFirst difference at byte: 4");
}

}
