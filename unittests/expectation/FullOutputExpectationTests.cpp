/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "unittests/test_framework.hpp"

#include "headers/expectation/FullOutputExpectation.hpp"
#include "headers/ProcessResults.hpp"

#include <algorithm>


namespace omtt
{

namespace
{

bool
contain(const std::string &text, const std::string &value)
{
    return text.find(value) != std::string::npos;
}

}

TEST_CASE("Should be satisfied when expected output and SUT output is the same")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "some output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == true);
}

TEST_CASE("Should not be satisfied when expected output and SUT output is not the same")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "some other output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
}

TEST_CASE("Should not be satisfied when expected output and SUT output differs in letters case")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "Some Output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
}

TEST_CASE("Cause should not be set when expectation match")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "some output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == true);
    CHECK(!validationReults.cause.has_value());
}

TEST_CASE("Cause should be set when expectation didn't match")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "SOME OUTPUT"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
    CHECK(validationReults.cause.has_value());
}

TEST_CASE("Cause should not be set on empty texts")
{
    const std::string expectedOutput = "";
    const ProcessResults sutResults {0, ""};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == true);
    CHECK(!validationReults.cause.has_value());
}

TEST_CASE("Cause should point to first byte when the difference is on the first position")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "Some Output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "Output doesn't match.\nFirst difference at byte: 0"));
}

TEST_CASE("Cause should point to fourth byte when the difference is on the fourth position")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "somE output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "Output doesn't match.\nFirst difference at byte: 3"));
}

TEST_CASE("Cause should point to last byte when the difference is on the last position")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "some outpuT"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "Output doesn't match.\nFirst difference at byte: 10"));
}

TEST_CASE("Cause should point after last byte of expectation text when expectation text is shorten than SUT output")
{
    const std::string expectedOutput = "some";
    const ProcessResults sutResults {0, "some text"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "Output doesn't match.\nFirst difference at byte: 4"));
}

TEST_CASE("Cause should point after last byte of SUT output text when SUT output text is shorten than expectation text")
{
    const std::string expectedOutput = "some text";
    const ProcessResults sutResults {0, "some"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "Output doesn't match.\nFirst difference at byte: 4"));
}

TEST_CASE("Cause should have context and pointer to first byte when the difference is on the first position")
{
    const std::string expectedOutput = "some output";
    const ProcessResults sutResults {0, "Some Output"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
s    o    m    e         o    u    \n\
^                                  \n\
0x73 0x6f 0x6d 0x65 0x20 0x6f 0x75 \n\
Output context:\n\
S    o    m    e         O    u    \n\
^                                  \n\
0x53 0x6f 0x6d 0x65 0x20 0x4f 0x75 "));
}

TEST_CASE("Cause should have context and pointer to second byte when the difference is on the second position")
{
    const std::string expectedOutput = "some";
    const ProcessResults sutResults {0, "sOme"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
s    o    m    e    \n\
     ^              \n\
0x73 0x6f 0x6d 0x65 \n\
Output context:\n\
s    O    m    e    \n\
     ^              \n\
0x73 0x4f 0x6d 0x65 "));
}

TEST_CASE("Cause should have context and pointer to sixth byte when the difference is on the sixth position")
{
    const std::string expectedOutput = "some T";
    const ProcessResults sutResults {0, "some t"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
s    o    m    e         T    \n\
                         ^    \n\
0x73 0x6f 0x6d 0x65 0x20 0x54 \n\
Output context:\n\
s    o    m    e         t    \n\
                         ^    \n\
0x73 0x6f 0x6d 0x65 0x20 0x74 "));
}

TEST_CASE("Cause context should have context with fifth characters after wrong character")
{
    const std::string expectedOutput = "some verry long ";
    const ProcessResults sutResults {0, "some verry"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
     v    e    r    r    y         l    o    n    g         \n\
                              ^                             \n\
0x20 0x76 0x65 0x72 0x72 0x79 0x20 0x6c 0x6f 0x6e 0x67 0x20 \n\
Output context:\n\
     v    e    r    r    y    \n\
                              \n\
0x20 0x76 0x65 0x72 0x72 0x79 "));
}

TEST_CASE("Cause context should have context with six characters before wrong character")
{
    const std::string expectedOutput = "some verry long text";
    const ProcessResults sutResults {0, "some verry"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
     v    e    r    r    y         l    o    n    g         t    \n\
                              ^                                  \n\
0x20 0x76 0x65 0x72 0x72 0x79 0x20 0x6c 0x6f 0x6e 0x67 0x20 0x74 \n\
Output context:\n\
     v    e    r    r    y    \n\
                              \n\
0x20 0x76 0x65 0x72 0x72 0x79 "));
}

TEST_CASE("Cause context should have six characters after wrong character")
{
    const std::string expectedOutput = "some verry";
    const ProcessResults sutResults {0, "some verry long text"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
     v    e    r    r    y    \n\
                              \n\
0x20 0x76 0x65 0x72 0x72 0x79 \n\
Output context:\n\
     v    e    r    r    y         l    o    n    g         t    \n\
                              ^                                  \n\
0x20 0x76 0x65 0x72 0x72 0x79 0x20 0x6c 0x6f 0x6e 0x67 0x20 0x74 "));
}

}
