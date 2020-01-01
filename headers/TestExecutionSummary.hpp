/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/Verdict.hpp"

#include <ostream>


namespace omtt
{

struct TestExecutionSummary
{
    Verdict verdict;
};

std::ostream&
operator<<(std::ostream &os, const TestExecutionSummary &tes);

}  // omtt
