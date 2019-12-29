/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/expectation/Expectation.hpp"

#include <string_view>


namespace omtt::expectation
{

class FullOutputExpectation : public Expectation
{
public:
    explicit FullOutputExpectation(const std::string_view &expectedOutput)
        :
        fExpectedOutput(expectedOutput)
    {
    }

    bool
    IsSatisfied(const std::string &output) const
    {
        return fExpectedOutput == output;
    }

    const std::string_view &
    GetContent() const
    {
        return fExpectedOutput;
    }

private:
    const std::string_view fExpectedOutput;
};

}
