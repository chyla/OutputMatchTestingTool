/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/Path.hpp"
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
    virtual void EndTestExecution(const omtt::TestExecutionSummary &summary) = 0;

    virtual void OverallStatistics(const omtt::TestPaths::size_type executedTests,
                                   const omtt::TestPaths::size_type numberOfTestsPassed,
                                   const omtt::TestPaths::size_type numberOfTestsFailed) = 0;
};

}
