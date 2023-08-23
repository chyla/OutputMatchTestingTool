/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
