/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
    void EndTestExecution(const omtt::ProcessResults &processResults,
                          const omtt::TestExecutionSummary &summary) override;

    void OverallStatistics(const omtt::TestPaths::size_type executedTests,
                           const omtt::TestPaths::size_type numberOfTestsPassed,
                           const omtt::TestPaths::size_type numberOfTestsFailed) override;

private:
     std::ostream &stream;
};

}
