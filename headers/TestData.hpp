/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "headers/expectation/Expectation.hpp"

#include <memory>
#include <string_view>
#include <vector>


namespace omtt
{

struct TestData
{
    std::string_view input;
    std::vector<std::unique_ptr<expectation::Expectation>> expectations;
};

}  // omtt
