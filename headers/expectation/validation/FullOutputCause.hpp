/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <string_view>


namespace omtt::expectation::validation
{

struct FullOutputCause
{
    const std::string::size_type fDifferencePosition;
    const std::string_view fExpectedOutput;
    const std::string_view fOutput;
};

}
