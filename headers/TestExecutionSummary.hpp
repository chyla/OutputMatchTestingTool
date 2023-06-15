/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/Verdict.hpp"
#include "headers/expectation/validation/ValidationResult.hpp"

#include <ostream>
#include <vector>


namespace omtt
{

struct TestExecutionSummary
{
    Verdict verdict;
    std::vector<expectation::validation::ValidationResult::Cause> causes;
};

}  // omtt
