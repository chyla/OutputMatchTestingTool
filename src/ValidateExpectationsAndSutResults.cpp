/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
