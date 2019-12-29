/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "unittests/test_framework.hpp"

#include "headers/expectation/FullOutputExpectation.hpp"


namespace omtt
{

TEST_CASE("Should be satisfied when expected output and SUT output is the same")
{
    std::string expectedOutput = "some output";
    std::string sutOutput = "some output";

    expectation::FullOutputExpectation expectation(expectedOutput);

    CHECK(expectation.IsSatisfied(sutOutput) == true);
}

TEST_CASE("Should not be satisfied when expected output and SUT output is not the same")
{
    std::string expectedOutput = "some output";
    std::string sutOutput = "some other output";

    expectation::FullOutputExpectation expectation(expectedOutput);

    CHECK(expectation.IsSatisfied(sutOutput) == false);
}

TEST_CASE("Should not be satisfied when expected output and SUT output differs in letters case")
{
    std::string expectedOutput = "some output";
    std::string sutOutput = "Some Output";

    expectation::FullOutputExpectation expectation(expectedOutput);

    CHECK(expectation.IsSatisfied(sutOutput) == false);
}

}
