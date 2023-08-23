/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "headers/Path.hpp"
#include "headers/ProcessResults.hpp"
#include "headers/TestExecutionSummary.hpp"

#include <string>


namespace omtt::logger
{

class Logger
{
public:
    virtual      ~Logger() = default;

    virtual void SutPath(const std::string &) = 0;

    virtual void BeginTestExecution(const omtt::TestPaths::size_type executedTests,
                                    const omtt::TestPaths::size_type numberOfTests,
                                    const omtt::Path &testPath) = 0;
    virtual void EndTestExecution(const omtt::ProcessResults &processResults,
                                  const omtt::TestExecutionSummary &summary) = 0;

    virtual void OverallStatistics(const omtt::TestPaths::size_type executedTests,
                                   const omtt::TestPaths::size_type numberOfTestsPassed,
                                   const omtt::TestPaths::size_type numberOfTestsFailed) = 0;
};

}
