/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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

    CHECK(validationResult.isSatisfied() == true);
}


TEST_CASE("Should not be satisfied when output is not empty")
{
    const char * const notEmptyOutput = "some output";
    constexpr int notImportantExitCode = 0;

    const ProcessResults sutResults {notImportantExitCode, notEmptyOutput};
    EmptyOutputExpectation expectation;

    auto validationResult = expectation.Validate(sutResults);

    CHECK(validationResult.isSatisfied() == false);
}

}
