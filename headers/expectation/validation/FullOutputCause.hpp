/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
