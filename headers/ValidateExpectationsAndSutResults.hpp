/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/TestData.hpp"
#include "headers/ProcessResults.hpp"
#include "headers/TestExecutionSummary.hpp"


namespace omtt
{

TestExecutionSummary
ValidateExpectationsAndSutResults(const TestData&,
                                  const ProcessResults&);

}  // omtt
