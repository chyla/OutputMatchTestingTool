/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "headers/expectation/FullOutputExpectation.hpp"
#include "headers/expectation/validation/FullOutputCause.hpp"

#include <algorithm>
#include <string>
#include <string_view>


namespace omtt::expectation
{

namespace
{

std::string::size_type
find_first_difference_position(const std::string_view &expectedOutput, const std::string &output)
{
    auto diff = std::mismatch(output.begin(), output.end(), expectedOutput.begin());
    return std::distance(output.begin(), diff.first);
}

}

validation::ValidationResult
FullOutputExpectation::Validate(const ProcessResults &processResults)
{
    if (fExpectedOutput == processResults.output) {
        return {std::nullopt};
    }
    else {
        const auto differencePosition = find_first_difference_position(fExpectedOutput,
                                                                       processResults.output);

        return {expectation::validation::FullOutputCause{differencePosition,
                                                         fExpectedOutput,
                                                         processResults.output}};
    }
}

}  // omtt::expectation
