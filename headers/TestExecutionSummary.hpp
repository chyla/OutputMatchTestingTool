/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/Verdict.hpp"
#include "headers/expectation/ValidationResult.hpp"

#include <ostream>
#include <vector>


namespace omtt
{

struct TestExecutionSummary
{
    Verdict verdict;
    std::vector<expectation::ValidationResult::Cause> causes;
};

std::ostream&
operator<<(std::ostream &os, const TestExecutionSummary &tes);

}  // omtt
