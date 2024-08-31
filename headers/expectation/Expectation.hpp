/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
