/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
