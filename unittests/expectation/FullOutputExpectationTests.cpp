/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
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

TEST_CASE("Context should have three empty new lines for empty process output")
{
    const std::string expectedOutput = "a";
    const ProcessResults sutResults {0, ""};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
a    \n\
^    \n\
0x61 \n\
Output context:\n\
\n\
\n\
"));
}

TEST_CASE("Context should have three empty new lines for empty expected output")
{
    const std::string expectedOutput = "";
    const ProcessResults sutResults {0, "a"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
\n\
\n\
\n\
Output context:\n\
a    \n\
^    \n\
0x61"));
}

TEST_CASE("Should change space to SPC in cause")
{
    const std::string expectedOutput = " ";
    const ProcessResults sutResults {0, " a"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
SPC  \n\
     \n\
0x20 \n\
Output context:\n\
SPC  a    \n\
     ^    \n\
0x20 0x61"));
}

TEST_CASE("Should change horizontal tab to TAB in cause")
{
    const std::string expectedOutput = "\t";
    const ProcessResults sutResults {0, "\ta"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
TAB  \n\
     \n\
0x09 \n\
Output context:\n\
TAB  a    \n\
     ^    \n\
0x09 0x61"));
}

TEST_CASE("Should change carriage return to CR in cause")
{
    const std::string expectedOutput = "\r";
    const ProcessResults sutResults {0, "\ra"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
CR   \n\
     \n\
0x0d \n\
Output context:\n\
CR   a    \n\
     ^    \n\
0x0d 0x61"));
}

TEST_CASE("Should change line feed to LF in cause")
{
    const std::string expectedOutput = "\n";
    const ProcessResults sutResults {0, "\na"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
LF   \n\
     \n\
0x0a \n\
Output context:\n\
LF   a    \n\
     ^    \n\
0x0a 0x61"));
}

TEST_CASE("Should change vertical tab to VT in cause")
{
    const std::string expectedOutput = "\v";
    const ProcessResults sutResults {0, "\va"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
VT   \n\
     \n\
0x0b \n\
Output context:\n\
VT   a    \n\
     ^    \n\
0x0b 0x61"));
}

TEST_CASE("Should change feed to FF in cause")
{
    const std::string expectedOutput = "\f";
    const ProcessResults sutResults {0, "\fa"};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
FF   \n\
     \n\
0x0c \n\
Output context:\n\
FF   a    \n\
     ^    \n\
0x0c 0x61"));
}

TEST_CASE("Should change non-printable characters to NP in cause")
{
    constexpr char firstPrintableCharacterInAsciiTable = 0x1f;
    constexpr char nonPrintableCharacter = firstPrintableCharacterInAsciiTable - 1;
    constexpr char nonPrintableString[2] {nonPrintableCharacter, 0x00};
    const std::string expectedOutput = "a";
    const ProcessResults sutResults {0, nonPrintableString};

    expectation::FullOutputExpectation expectation(expectedOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Expected context:\n\
a    \n\
^    \n\
0x61 \n\
Output context:\n\
NP   \n\
^    \n\
0x1e"));
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
s    o    m    e    SPC  o    u    \n\
^                                  \n\
0x73 0x6f 0x6d 0x65 0x20 0x6f 0x75 \n\
Output context:\n\
S    o    m    e    SPC  O    u    \n\
^                                  \n\
0x53 0x6f 0x6d 0x65 0x20 0x4f 0x75"));
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
0x73 0x4f 0x6d 0x65"));
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
s    o    m    e    SPC  T    \n\
                         ^    \n\
0x73 0x6f 0x6d 0x65 0x20 0x54 \n\
Output context:\n\
s    o    m    e    SPC  t    \n\
                         ^    \n\
0x73 0x6f 0x6d 0x65 0x20 0x74"));
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
SPC  v    e    r    r    y    SPC  l    o    n    g    SPC  \n\
                              ^                             \n\
0x20 0x76 0x65 0x72 0x72 0x79 0x20 0x6c 0x6f 0x6e 0x67 0x20 \n\
Output context:\n\
SPC  v    e    r    r    y    \n\
                              \n\
0x20 0x76 0x65 0x72 0x72 0x79"));
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
SPC  v    e    r    r    y    SPC  l    o    n    g    SPC  t    \n\
                              ^                                  \n\
0x20 0x76 0x65 0x72 0x72 0x79 0x20 0x6c 0x6f 0x6e 0x67 0x20 0x74 \n\
Output context:\n\
SPC  v    e    r    r    y    \n\
                              \n\
0x20 0x76 0x65 0x72 0x72 0x79"));
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
SPC  v    e    r    r    y    \n\
                              \n\
0x20 0x76 0x65 0x72 0x72 0x79 \n\
Output context:\n\
SPC  v    e    r    r    y    SPC  l    o    n    g    SPC  t    \n\
                              ^                                  \n\
0x20 0x76 0x65 0x72 0x72 0x79 0x20 0x6c 0x6f 0x6e 0x67 0x20 0x74"));
}

}
