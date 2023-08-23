/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "headers/expectation/Expectation.hpp"
#include "headers/expectation/validation/SuccessfulExitCause.hpp"


namespace omtt::expectation
{

class SuccessfulExitExpectation : public Expectation
{
public:
    validation::ValidationResult
    Validate(const ProcessResults &processResults)
    {
        if (processResults.exitCode == 0) {
            return {std::nullopt};
        }
        else {
            return {validation::SuccessfulExitCause{processResults.exitCode}};
        }
    }
};

}
