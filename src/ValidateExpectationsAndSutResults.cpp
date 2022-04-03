/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/ValidateExpectationsAndSutResults.hpp"


namespace omtt
{

TestExecutionSummary
ValidateExpectationsAndSutResults(const TestData &testData,
                                  const ProcessResults &processResults)
{
    TestExecutionSummary summary;
    summary.verdict = Verdict::PASS;

    for (const auto &expectation : testData.expectations) {
        auto validationResult = expectation->Validate(processResults);

        if (!validationResult.isSatisfied()) {
            summary.verdict = Verdict::FAIL;
            summary.causes.push_back(*validationResult.cause);
        }
    }

    return summary;
}

}  // omtt
