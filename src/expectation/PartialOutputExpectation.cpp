/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/expectation/PartialOutputExpectation.hpp"
#include "headers/expectation/detail/Context.hpp"

#include <algorithm>
#include <string>


namespace omtt::expectation
{


validation::ValidationResult
PartialOutputExpectation::Validate(const ProcessResults &processResults)
{
    validation::ValidationResult result;

    const auto pos = processResults.output.find(fExpectedPartialOutput);

    if (pos != std::string::npos) {
        result.isSatisfied = true;
        result.cause = std::nullopt;
    }
    else {
        constexpr auto differencePosition = 0;

        result.isSatisfied = false;
        result.cause =
            "Text not found in output.\n"
            "Given text context:\n"
          + detail::context(fExpectedPartialOutput,
                            differencePosition,
                            detail::PointerVisibility::NO_POINTER);
    }

    return result;
}

}  // omtt::expectation
