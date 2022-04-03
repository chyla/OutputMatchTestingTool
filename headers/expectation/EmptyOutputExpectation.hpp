/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
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
