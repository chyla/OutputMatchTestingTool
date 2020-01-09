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

    CHECK(expectation.IsSatisfied(sutResults) == true);
}

TEST_CASE("Should not be satisfied when expected output and SUT output is not the same")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "some other output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    CHECK(expectation.IsSatisfied(sutResults) == false);
}

TEST_CASE("Should not be satisfied when expected output and SUT output differs in letters case")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "Some Output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    CHECK(expectation.IsSatisfied(sutResults) == false);
}

}
