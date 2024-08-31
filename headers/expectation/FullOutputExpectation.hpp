/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
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

class FullOutputExpectation : public Expectation
{
public:
    explicit FullOutputExpectation(const std::string_view &expectedOutput)
        :
        fExpectedOutput(expectedOutput)
    {
    }

    validation::ValidationResult Validate(const ProcessResults &processResults);

    const std::string_view &
    GetContent() const
    {
        return fExpectedOutput;
    }

private:
    const std::string_view fExpectedOutput;
};

}
