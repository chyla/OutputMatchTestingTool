/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/expectation/Expectation.hpp"

#include <algorithm>
#include <iterator>
#include <string_view>


namespace omtt::expectation
{

class FullOutputExpectation : public Expectation
{
public:
    explicit FullOutputExpectation(const std::string_view &expectedOutput)
        :
        fExpectedOutput(expectedOutput)
    {
    }

    ValidationResult
    Validate(const ProcessResults &processResults)
    {
        ValidationResult result;

        if (fExpectedOutput == processResults.output) {
            result.isSatisfied = true;
            result.cause = std::nullopt;
        }
        else {
            result.isSatisfied = false;
            result.cause =
                "Output doesn't match.\n"
                "First difference at byte: " + std::to_string(_FindFirstDifferencePosition(fExpectedOutput, processResults.output));
        }

        return result;
    }

    const std::string_view &
    GetContent() const
    {
        return fExpectedOutput;
    }

private:
    const std::string_view fExpectedOutput;

    static std::string::size_type
    _FindFirstDifferencePosition(const std::string_view &expectedOutput, const std::string &output)
    {
        auto diff = std::mismatch(output.begin(), output.end(), expectedOutput.begin());
        return std::distance(output.begin(), diff.first);
    }
};

}
