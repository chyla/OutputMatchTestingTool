/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
