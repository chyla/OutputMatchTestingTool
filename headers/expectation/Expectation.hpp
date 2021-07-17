/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/ProcessResults.hpp"
#include "headers/expectation/validation/ValidationResult.hpp"


namespace omtt::expectation
{

class Expectation
{
public:
    virtual                               ~Expectation() = default;

    virtual validation::ValidationResult  Validate(const ProcessResults &processResults) = 0;
};

}
