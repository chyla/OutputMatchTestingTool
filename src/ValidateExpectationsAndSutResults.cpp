/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/ValidateExpectationsAndSutResults.hpp"


namespace omtt
{

const TestExecutionSummary
ValidateExpectationsAndSutResults(const TestData &testData,
                                  const ProcessResults &processResults)
{
    TestExecutionSummary summary;
    summary.verdict = Verdict::PASS;

    for (const auto &expectation : testData.expectations) {
        if (!expectation->IsSatisfied(processResults.output)) {
            summary.verdict = Verdict::FAIL;
        }
    }

    return summary;
}

}  // omtt
