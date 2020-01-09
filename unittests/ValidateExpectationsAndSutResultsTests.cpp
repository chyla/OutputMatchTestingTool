/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "unittests/test_framework.hpp"

#include "headers/expectation/Expectation.hpp"
#include "headers/ValidateExpectationsAndSutResults.hpp"
#include "headers/ProcessResults.hpp"


namespace omtt
{

namespace
{

class TrueExpectation : public expectation::Expectation
{
public:
    bool
    IsSatisfied(const ProcessResults &output) const
    {
        return true;
    }
};


class FalseExpectation : public expectation::Expectation
{
public:
    bool
    IsSatisfied(const ProcessResults &output) const
    {
        return false;
    }
};

void
AppendSatisfiedExpectation(TestData &testData)
{
    testData.expectations.push_back(std::move(std::make_unique<TrueExpectation>()));
}

void
AppendNotSatisfiedExpectation(TestData &testData)
{
    testData.expectations.push_back(std::move(std::make_unique<FalseExpectation>()));
}

}


TEST_CASE("Should set test verdict to PASS when no expectations are given")
{
    TestData testData;
    ProcessResults processResults;

    TestExecutionSummary summary = ValidateExpectationsAndSutResults(testData, processResults);

    CHECK(summary.verdict == Verdict::PASS);
}

TEST_CASE("Should set test verdict to PASS when only satisfied expectations are given")
{
    TestData testData;
    AppendSatisfiedExpectation(testData);
    AppendSatisfiedExpectation(testData);
    ProcessResults processResults;

    TestExecutionSummary summary = ValidateExpectationsAndSutResults(testData, processResults);

    CHECK(summary.verdict == Verdict::PASS);
}

TEST_CASE("Should set test verdict to FAIL when one not satisfied expectation is given")
{
    TestData testData;
    AppendNotSatisfiedExpectation(testData);
    ProcessResults processResults;

    TestExecutionSummary summary = ValidateExpectationsAndSutResults(testData, processResults);

    CHECK(summary.verdict == Verdict::FAIL);
}

TEST_CASE("Should set test verdict to FAIL when one of expectations is not satisfied")
{
    TestData testData;
    AppendSatisfiedExpectation(testData);
    AppendSatisfiedExpectation(testData);
    AppendNotSatisfiedExpectation(testData);
    AppendSatisfiedExpectation(testData);
    ProcessResults processResults;

    TestExecutionSummary summary = ValidateExpectationsAndSutResults(testData, processResults);

    CHECK(summary.verdict == Verdict::FAIL);
}

}
