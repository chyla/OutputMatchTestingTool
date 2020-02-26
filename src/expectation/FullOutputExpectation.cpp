/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/expectation/FullOutputExpectation.hpp"
#include "headers/expectation/detail/Context.hpp"

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

ValidationResult
FullOutputExpectation::Validate(const ProcessResults &processResults)
{
    ValidationResult result;

    if (fExpectedOutput == processResults.output) {
        result.isSatisfied = true;
        result.cause = std::nullopt;
    }
    else {
        const auto differencePosition = find_first_difference_position(fExpectedOutput, processResults.output);
        result.isSatisfied = false;
        result.cause =
            "Output doesn't match.\n"
            "First difference at byte: " + std::to_string(differencePosition) + "\n"
          + "Expected context:\n" + detail::context(fExpectedOutput, differencePosition, detail::PointerVisibility::INCLUDE_POINTER) + "\n"
          + "Output context:\n" + detail::context(processResults.output, differencePosition, detail::PointerVisibility::INCLUDE_POINTER);
    }

    return result;
}

}  // omtt::expectation
