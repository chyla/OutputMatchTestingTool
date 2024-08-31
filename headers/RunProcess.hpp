/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "headers/ProcessResults.hpp"

#include <string_view>
#include <vector>


namespace omtt
{

ProcessResults
RunProcess(const std::string &path,
           const std::vector<std::string> &options,
           const std::string_view &input);

}  // omtt
