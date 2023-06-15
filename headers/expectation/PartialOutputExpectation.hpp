/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/expectation/Expectation.hpp"

#include <string_view>


namespace omtt::expectation
{

class PartialOutputExpectation : public Expectation
{
public:
    explicit PartialOutputExpectation(const std::string_view &expectedPartialOutput)
        :
        fExpectedPartialOutput(expectedPartialOutput)
    {
    }

    validation::ValidationResult Validate(const ProcessResults &processResults);

    const std::string_view &
    GetContent() const
    {
        return fExpectedPartialOutput;
    }

private:
    const std::string_view fExpectedPartialOutput;
};

}
