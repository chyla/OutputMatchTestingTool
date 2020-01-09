/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/ProcessResults.hpp"


namespace omtt::expectation
{

class Expectation
{
public:
    virtual         ~Expectation() = default;

    virtual bool    IsSatisfied(const ProcessResults &processResults) const = 0;
};

}
