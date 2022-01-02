/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/logger/Logger.hpp"
#include <iostream>


namespace omtt::logger
{

class ConsoleLogger : public Logger
{
public:
         ConsoleLogger(std::ostream &stream = std::cout) : stream(stream) {};
         ~ConsoleLogger() = default;

    void SutPath(const std::string &) override;

    void BeginTestExecution(const omtt::TestPaths::size_type executedTests,
                            const omtt::TestPaths::size_type numberOfTests,
                            const omtt::Path &testPath) override;
    void EndTestExecution(const omtt::TestExecutionSummary &summary) override;

    void OverallStatistics(const omtt::TestPaths::size_type executedTests,
                           const omtt::TestPaths::size_type numberOfTestsPassed,
                           const omtt::TestPaths::size_type numberOfTestsFailed) override;

private:
     std::ostream &stream;
};

}
