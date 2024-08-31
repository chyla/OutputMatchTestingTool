/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "headers/expectation/PartialOutputExpectation.hpp"
#include "headers/expectation/validation/PartialOutputCause.hpp"

#include <algorithm>
#include <string>


namespace omtt::expectation
{


validation::ValidationResult
PartialOutputExpectation::Validate(const ProcessResults &processResults)
{
    const auto pos = processResults.output.find(fExpectedPartialOutput);

    if (pos != std::string::npos) {
        return {std::nullopt};
    }
    else {
        return {validation::PartialOutputCause{fExpectedPartialOutput}};
    }
}

}  // omtt::expectation
