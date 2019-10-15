/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <string_view>


namespace omtt
{

struct TestData
{
    std::string_view input;
    std::string_view expectedOutput;
};

}  // omtt
