/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
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

    bool
    IsSatisfied(const ProcessResults &processResults) const
    {
        return fExpectedExitCode == processResults.exitCode;
    }

    const int
    GetContent() const
    {
        return fExpectedExitCode;
    }

private:
    const int fExpectedExitCode;
};

}
