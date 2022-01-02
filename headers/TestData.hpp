/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
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
