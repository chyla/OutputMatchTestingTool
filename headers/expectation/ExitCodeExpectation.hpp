/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "headers/expectation/Expectation.hpp"
#include "headers/expectation/validation/ExitCodeCause.hpp"


namespace omtt::expectation
{

class ExitCodeExpectation : public Expectation
{
public:
    explicit ExitCodeExpectation(const int expectedExitCode)
        :
        fExpectedExitCode(expectedExitCode)
    {
    }

    validation::ValidationResult
    Validate(const ProcessResults &processResults)
    {
        if (fExpectedExitCode == processResults.exitCode) {
            return {std::nullopt};
        }
        else {
            return {validation::ExitCodeCause{fExpectedExitCode, processResults.exitCode}};
        }
    }

    int
    GetContent() const
    {
        return fExpectedExitCode;
    }

private:
    const int fExpectedExitCode;
};

}
