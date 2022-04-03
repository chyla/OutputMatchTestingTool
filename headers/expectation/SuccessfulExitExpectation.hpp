/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
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
