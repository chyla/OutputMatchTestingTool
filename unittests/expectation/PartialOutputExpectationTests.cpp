/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "unittests/test_framework.hpp"

#include "headers/expectation/PartialOutputExpectation.hpp"
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

TEST_CASE("Should be satisfied when SUT output contains given text")
{
    const std::string expectedPartialOutput = "me ou";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == true);
}

TEST_CASE("Should be satisfied on empty texts")
{
    const std::string expectedPartialOutput = "";
    const ProcessResults sutResults {0, ""};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == true);
}

TEST_CASE("Should be satisfied when expected partial output is empty text")
{
    const std::string expectedPartialOutput = "";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == true);
}

TEST_CASE("Should not be satisfied when SUT output doesn't contain given text")
{
    const std::string expectedPartialOutput = "other text";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
}

TEST_CASE("Should not be satisfied when SUT output and given text differs in letters case")
{
    const std::string expectedPartialOutput = "somE output";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
}

TEST_CASE("Cause should not be set when expectation match")
{
    const std::string expectedPartialOutput = "me ou";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == true);
    CHECK(!validationReults.cause.has_value());
}

TEST_CASE("Cause should be set when expectation didn't match")
{
    const std::string expectedPartialOutput = "other text";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
    CHECK(validationReults.cause.has_value());
}

TEST_CASE("Cause should contain failure desctiption")
{
    const std::string expectedPartialOutput = "other text";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "Text not found in output."));
}

TEST_CASE("Cause should contain text and hex context up to six bytes when expected partial text is longer")
{
    const std::string expectedPartialOutput = "some long text";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Given text context:\n\
s    o    m    e    SPC  l    o    \n\
0x73 0x6f 0x6d 0x65 0x20 0x6c 0x6f"));
}

TEST_CASE("Cause should contain text and hex context up to five bytes when expected partial text have five bytes")
{
    const std::string expectedPartialOutput = "Some ";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Given text context:\n\
S    o    m    e    SPC  \n\
0x53 0x6f 0x6d 0x65 0x20"));
}

TEST_CASE("Cause should contain one byte of text and hex context up when expected partial text have one byte")
{
    const std::string expectedPartialOutput = "S";
    const ProcessResults sutResults {0, "some output"};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.isSatisfied == false);
    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Given text context:\n\
S    \n\
0x53"));
}

TEST_CASE("Should change space to SPC in cause")
{
    const std::string expectedPartialOutput = " a";
    const ProcessResults sutResults {0, ""};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Given text context:\n\
SPC  a    \n\
0x20 0x61"));
}

TEST_CASE("Should change horizontal tab to TAB in cause")
{
    const std::string expectedPartialOutput = "\ta";
    const ProcessResults sutResults {0, ""};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Given text context:\n\
TAB  a    \n\
0x09 0x61"));
}

TEST_CASE("Should change carriage return to CR in cause")
{
    const std::string expectedPartialOutput = "\ra";
    const ProcessResults sutResults {0, ""};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Given text context:\n\
CR   a    \n\
0x0d 0x61"));
}

TEST_CASE("Should change line feed to LF in cause")
{
    const std::string expectedPartialOutput = "\na";
    const ProcessResults sutResults {0, ""};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Given text context:\n\
LF   a    \n\
0x0a 0x61"));
}

TEST_CASE("Should change vertical tab to VT in cause")
{
    const std::string expectedPartialOutput = "\va";
    const ProcessResults sutResults {0, ""};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Given text context:\n\
VT   a    \n\
0x0b 0x61"));
}

TEST_CASE("Should change feed to FF in cause")
{
    const std::string expectedPartialOutput = "\fa";
    const ProcessResults sutResults {0, ""};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Given text context:\n\
FF   a    \n\
0x0c 0x61"));
}

TEST_CASE("Should change non-printable characters to NP in cause")
{
    constexpr char firstPrintableCharacterInAsciiTable = 0x1f;
    constexpr char nonPrintableCharacter = firstPrintableCharacterInAsciiTable - 1;
    constexpr char nonPrintableString[2] {nonPrintableCharacter, 0x00};
    const std::string expectedPartialOutput = nonPrintableString;
    const ProcessResults sutResults {0, ""};

    expectation::PartialOutputExpectation expectation(expectedPartialOutput);

    auto validationReults = expectation.Validate(sutResults);

    CHECK(validationReults.cause.has_value());
    CHECK(contain(*validationReults.cause, "\
Given text context:\n\
NP   \n\
0x1e"));
}

}
