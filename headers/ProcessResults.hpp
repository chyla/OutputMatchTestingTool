/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <string>


namespace omtt
{

struct ProcessResults
{
    int exitCode;
    std::string output;
    std::string errors;
};

}  // omtt
