/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/ProcessResults.hpp"

#include <string_view>


namespace omtt
{

ProcessResults
RunProcess(const std::string &path, const std::string_view &input);

}  // omtt
