/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/expectation/validation/ExitCodeCause.hpp"
#include "headers/expectation/validation/FullOutputCause.hpp"
#include "headers/expectation/validation/PartialOutputCause.hpp"

#include <string>
#include <optional>
#include <variant>


namespace omtt::expectation::validation
{

struct ValidationResult
{
    typedef std::variant<validation::ExitCodeCause,
                         validation::FullOutputCause,
                         validation::PartialOutputCause> Cause;

    const bool isSatisfied;
    const std::optional<Cause> cause;
};

}
