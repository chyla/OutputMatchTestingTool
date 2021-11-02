/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "unittests/test_framework.hpp"

#include "headers/expectation/EmptyOutputExpectation.hpp"
#include "headers/ProcessResults.hpp"


namespace omtt::expectation
{

TEST_CASE("Should be satisfied when output is empty")
{
    const char * const emptyOutput = "";
    constexpr int notImportantExitCode = 0;

    const ProcessResults sutResults {notImportantExitCode, emptyOutput};
    EmptyOutputExpectation expectation;

    auto validationResult = expectation.Validate(sutResults);

    CHECK(validationResult.isSatisfied == true);
}


TEST_CASE("Should not be satisfied when output is not empty")
{
    const char * const notEmptyOutput = "some output";
    constexpr int notImportantExitCode = 0;

    const ProcessResults sutResults {notImportantExitCode, notEmptyOutput};
    EmptyOutputExpectation expectation;

    auto validationResult = expectation.Validate(sutResults);

    CHECK(validationResult.isSatisfied == false);
}

}
