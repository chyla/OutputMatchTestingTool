/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
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
        return {true, std::nullopt};
    }
    else {
        const auto differencePosition = find_first_difference_position(fExpectedOutput,
                                                                       processResults.output);

        return {false, expectation::validation::FullOutputCause{differencePosition,
                                                                fExpectedOutput,
                                                                processResults.output}};
    }
}

}  // omtt::expectation
