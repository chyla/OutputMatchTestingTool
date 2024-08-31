/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "headers/expectation/Expectation.hpp"
#include "headers/expectation/validation/EmptyOutputCause.hpp"


namespace omtt::expectation
{

class EmptyOutputExpectation : public Expectation
{
public:
    explicit EmptyOutputExpectation() = default;

    validation::ValidationResult
    Validate(const ProcessResults &processResults)
    {
        if (processResults.output.empty()) {
            return {std::nullopt};
        }
        else {
            return {validation::EmptyOutputCause{processResults.output}};
        }
    }
};

}
