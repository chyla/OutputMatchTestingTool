/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/expectation/Expectation.hpp"


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
        validation::ValidationResult result;

        if (fExpectedExitCode == processResults.exitCode) {
            result.isSatisfied = true;
            result.cause = std::nullopt;
        }
        else {
            result.isSatisfied = false;
            result.cause =
                "Exit code doesn't match.\n"
                "Expected: " + std::to_string(fExpectedExitCode) + "\n"
                "Got: " + std::to_string(processResults.exitCode);
        }

        return result;
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
