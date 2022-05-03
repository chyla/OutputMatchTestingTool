/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/expectation/validation/EmptyOutputCause.hpp"
#include "headers/expectation/validation/ExitCodeCause.hpp"
#include "headers/expectation/validation/FullOutputCause.hpp"
#include "headers/expectation/validation/PartialOutputCause.hpp"
#include "headers/expectation/validation/SuccessfulExitCause.hpp"
#include "headers/expectation/validation/FailureExitCause.hpp"

#include <string>
#include <optional>
#include <variant>


namespace omtt::expectation::validation
{

struct ValidationResult
{
    typedef std::variant<
        validation::EmptyOutputCause,
        validation::ExitCodeCause,
        validation::FullOutputCause,
        validation::PartialOutputCause,
        validation::SuccessfulExitCause,
        validation::FailureExitCause
        > Cause;

    const std::optional<Cause> cause;

    bool
    isSatisfied() const {
        return not cause.has_value();
    }
};

}
