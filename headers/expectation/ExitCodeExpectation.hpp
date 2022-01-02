/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
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
            return {true,
                    std::nullopt};
        }
        else {
            return {false,
                    validation::ExitCodeCause{fExpectedExitCode, processResults.exitCode}};
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
