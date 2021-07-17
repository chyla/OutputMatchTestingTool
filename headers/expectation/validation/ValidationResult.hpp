/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <string>
#include <optional>


namespace omtt::expectation::validation
{

struct ValidationResult
{
    typedef std::string Cause;

    bool isSatisfied;
    std::optional<Cause> cause;
};

}
