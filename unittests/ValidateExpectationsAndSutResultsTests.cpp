/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "unittests/test_framework.hpp"

#include "headers/expectation/Expectation.hpp"
#include "headers/expectation/validation/ExitCodeCause.hpp"
#include "headers/ValidateExpectationsAndSutResults.hpp"
#include "headers/ProcessResults.hpp"


namespace omtt
{

namespace
{

using SampleCauseType = expectation::validation::ExitCodeCause;
const SampleCauseType sampleCause{1, 2};

class TrueExpectation : public expectation::Expectation
{
public:
    expectation::validation::ValidationResult
    Validate(const ProcessResults &output)
    {
        return {std::nullopt};
    }
};


class FalseExpectation : public expectation::Expectation
{
public:
    expectation::validation::ValidationResult
    Validate(const ProcessResults &output)
    {
        return {sampleCause};
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

TEST_CASE("Should set causes to empty list when no expectations are given")
{
    TestData testData;
    ProcessResults processResults;

    TestExecutionSummary summary = ValidateExpectationsAndSutResults(testData, processResults);

    CHECK(summary.causes.empty());
}

TEST_CASE("Should set causes to empty list when only satisfied expectations are given")
{
    TestData testData;
    AppendSatisfiedExpectation(testData);
    AppendSatisfiedExpectation(testData);
    ProcessResults processResults;

    TestExecutionSummary summary = ValidateExpectationsAndSutResults(testData, processResults);

    CHECK(summary.causes.empty());
}

TEST_CASE("Should append cause when one not satisfied expectation is given")
{
    TestData testData;
    AppendNotSatisfiedExpectation(testData);
    ProcessResults processResults;

    TestExecutionSummary summary = ValidateExpectationsAndSutResults(testData, processResults);

    CHECK(summary.causes.size() == 1);
    CHECK(std::get<SampleCauseType>(summary.causes.at(0)).fExitCode == sampleCause.fExitCode);
    CHECK(std::get<SampleCauseType>(summary.causes.at(0)).fExpectedExitCode == sampleCause.fExpectedExitCode);
}

TEST_CASE("Should append multiple causes when multiple expectations is not satisfied")
{
    TestData testData;
    AppendSatisfiedExpectation(testData);
    AppendNotSatisfiedExpectation(testData);
    AppendSatisfiedExpectation(testData);
    AppendNotSatisfiedExpectation(testData);
    AppendSatisfiedExpectation(testData);
    ProcessResults processResults;

    TestExecutionSummary summary = ValidateExpectationsAndSutResults(testData, processResults);

    CHECK(summary.causes.size() == 2);
    CHECK(std::get<SampleCauseType>(summary.causes.at(0)).fExitCode == sampleCause.fExitCode);
    CHECK(std::get<SampleCauseType>(summary.causes.at(0)).fExpectedExitCode == sampleCause.fExpectedExitCode);
    CHECK(std::get<SampleCauseType>(summary.causes.at(1)).fExitCode == sampleCause.fExitCode);
    CHECK(std::get<SampleCauseType>(summary.causes.at(1)).fExpectedExitCode == sampleCause.fExpectedExitCode);
}

}
