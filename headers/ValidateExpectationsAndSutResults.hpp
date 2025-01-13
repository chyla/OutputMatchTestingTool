/*
 * Copyright (c) 2019-2025, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
