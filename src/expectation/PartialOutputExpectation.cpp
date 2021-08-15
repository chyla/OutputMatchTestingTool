/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
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
        return {true, std::nullopt};
    }
    else {
        return {false, validation::PartialOutputCause{fExpectedPartialOutput}};
    }
}

}  // omtt::expectation
