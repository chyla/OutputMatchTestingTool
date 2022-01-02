/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "unittests/test_framework.hpp"

#include "headers/logger/ConsoleLogger.hpp"
#include "headers/expectation/validation/FullOutputCause.hpp"

#include <sstream>


namespace omtt
{

namespace
{

bool
contain(const std::string &text, const std::string &value)
{
    return text.find(value) != std::string::npos;
}

std::string
ExecuteSut(const TestExecutionSummary &summary)
{
    std::stringstream stream;
    logger::ConsoleLogger sut(stream);
    sut.EndTestExecution(summary);
    return stream.str();
}

}

namespace exit_code
{

TestExecutionSummary
CreateTestSummary(const int expectedExitCode,
                  const int exitCode)
{
    auto cause = expectation::validation::ExitCodeCause{expectedExitCode, exitCode};

    return {
        Verdict::FAIL,
        {cause}
    };
}

}

TEST_GROUP("Exit Code Cause logging")
{

    UNIT_TEST("Context should have exit code mismatch message wit expected and got numbers")
    {
        constexpr int expectedExitCode = 2;
        constexpr int exitCode = 6;
        const TestExecutionSummary testSummary = exit_code::CreateTestSummary(expectedExitCode, exitCode);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "Exit code doesn't match.\nExpected: 2\nGot: 6"));
    }

}

namespace full_output
{

TestExecutionSummary
CreateTestSummary(const std::string::size_type differencePosition = 0,
                  const std::string &expectedOutput = "",
                  const std::string &output = "")
{
    auto cause = expectation::validation::FullOutputCause{differencePosition, expectedOutput, output};

    return {
        Verdict::FAIL,
        {cause}
    };
}

void
CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(const std::string &output,
                                       const std::string &char_text,
                                       const std::string &char_code)
{
    CHECK(contain(output, "Expected (context):\n" + char_text + "  \n\
     \n" + char_code + " \n\
Got (context):\n" + char_text + "  a    \n\
     ^    \n" + char_code + " 0x61"));
}

}

TEST_GROUP("Full Output Cause logging")
{

    UNIT_TEST("Difference position message should point to zero byte when the difference is on the zero position")
    {
        constexpr std::string::size_type differencePosition = 0;
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "Output doesn't match.\nFirst difference at byte: 0"));
    }

    UNIT_TEST("Difference position message should point to fourth byte when the difference is on the fourth position")
    {
        constexpr std::string::size_type differencePosition = 4;
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "Output doesn't match.\nFirst difference at byte: 4"));
    }

    UNIT_TEST("Context should have three empty new lines for empty process output")
    {
        constexpr std::string::size_type differencePosition = 0;
        const std::string expectedOutput = "a";
        const std::string output = "";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "\
Expected (context):\n\
a    \n\
^    \n\
0x61 \n\
Got (context):\n\
\n\
\n\
"));
    }

    UNIT_TEST("Context should have three empty new lines for empty expected output")
    {
        constexpr std::string::size_type differencePosition = 0;
        const std::string expectedOutput = "";
        const std::string output = "a";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "\
Expected (context):\n\
\n\
\n\
\n\
Got (context):\n\
a    \n\
^    \n\
0x61"));
    }

    UNIT_TEST("Context should have SPC instead of space")
    {
        constexpr std::string::size_type differencePosition = 1;
        const std::string expectedOutput = " ";
        const std::string output = " a";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        full_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "SPC", "0x20");
    }

    UNIT_TEST("Context should have TAB instead of horizontal tab")
    {
        constexpr std::string::size_type differencePosition = 1;
        const std::string expectedOutput = "\t";
        const std::string output = "\ta";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        full_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "TAB", "0x09");
    }

    UNIT_TEST("Context should have CR instead of carriage return")
    {
        constexpr std::string::size_type differencePosition = 1;
        const std::string expectedOutput = "\r";
        const std::string output = "\ra";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        full_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "CR ", "0x0d");
    }

    UNIT_TEST("Context should have LF instead of line feed")
    {
        constexpr std::string::size_type differencePosition = 1;
        const std::string expectedOutput = "\n";
        const std::string output = "\na";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        full_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "LF ", "0x0a");
    }

    UNIT_TEST("Context should have VT instead of line feed")
    {
        constexpr std::string::size_type differencePosition = 1;
        const std::string expectedOutput = "\v";
        const std::string output = "\va";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        full_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "VT ", "0x0b");
    }

    UNIT_TEST("Context should have FF instead of feed")
    {
        constexpr std::string::size_type differencePosition = 1;
        const std::string expectedOutput = "\f";
        const std::string output = "\fa";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        full_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "FF ", "0x0c");
    }

    UNIT_TEST("Context should have NP instead of non-printable character")
    {
        constexpr char nonPrintableCharacter = 0x1e;
        constexpr char nonPrintableExpected[] {nonPrintableCharacter, 0x00};
        constexpr char nonPrintableOutput[] {nonPrintableCharacter, 'a', 0x00};
        constexpr std::string::size_type differencePosition = 1;
        const std::string expectedOutput = nonPrintableExpected;
        const std::string output = nonPrintableOutput;
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        full_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "NP ", "0x1e");
    }

    UNIT_TEST("Context should have pointer to zero byte when the difference is on the zero position")
    {
        constexpr std::string::size_type differencePosition = 0;
        const std::string expectedOutput = "some output";
        const std::string output = "Some Output";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "\
Expected (context):\n\
s    o    m    e    SPC  o    u    \n\
^                                  \n\
0x73 0x6f 0x6d 0x65 0x20 0x6f 0x75 \n\
Got (context):\n\
S    o    m    e    SPC  O    u    \n\
^                                  \n\
0x53 0x6f 0x6d 0x65 0x20 0x4f 0x75"));
    }

    UNIT_TEST("Context should have pointer to first byte when the difference is on the first position")
    {
        constexpr std::string::size_type differencePosition = 1;
        const std::string expectedOutput = "some";
        const std::string output = "sOme";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "\
Expected (context):\n\
s    o    m    e    \n\
     ^              \n\
0x73 0x6f 0x6d 0x65 \n\
Got (context):\n\
s    O    m    e    \n\
     ^              \n\
0x73 0x4f 0x6d 0x65"));
    }

    UNIT_TEST("Context should have pointer to fifth byte when the difference is on the fifth position")
    {
        constexpr std::string::size_type differencePosition = 5;
        const std::string expectedOutput = "some T";
        const std::string output = "some t";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "\
Expected (context):\n\
s    o    m    e    SPC  T    \n\
                         ^    \n\
0x73 0x6f 0x6d 0x65 0x20 0x54 \n\
Got (context):\n\
s    o    m    e    SPC  t    \n\
                         ^    \n\
0x73 0x6f 0x6d 0x65 0x20 0x74"));
    }

    UNIT_TEST("Context should have five characters after wrong character when there is no more text")
    {
        constexpr std::string::size_type differencePosition = 10;
        const std::string expectedOutput = "some verry long ";
        const std::string output = "some verry";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "\
Expected (context):\n\
SPC  v    e    r    r    y    SPC  l    o    n    g    SPC  \n\
                              ^                             \n\
0x20 0x76 0x65 0x72 0x72 0x79 0x20 0x6c 0x6f 0x6e 0x67 0x20 \n\
Got (context):\n\
SPC  v    e    r    r    y    \n\
                              \n\
0x20 0x76 0x65 0x72 0x72 0x79"));
    }

    UNIT_TEST("Expected context should have six characters after wrong character")
    {
        constexpr std::string::size_type differencePosition = 10;
        const std::string expectedOutput = "some verry long text";
        const std::string output = "some verry";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "\
Expected (context):\n\
SPC  v    e    r    r    y    SPC  l    o    n    g    SPC  t    \n\
                              ^                                  \n\
0x20 0x76 0x65 0x72 0x72 0x79 0x20 0x6c 0x6f 0x6e 0x67 0x20 0x74 \n\
Got (context):\n\
SPC  v    e    r    r    y    \n\
                              \n\
0x20 0x76 0x65 0x72 0x72 0x79"));
    }

    UNIT_TEST("Output context should have six characters after wrong character")
    {
        constexpr std::string::size_type differencePosition = 10;
        const std::string expectedOutput = "some verry";
        const std::string output = "some verry long text";
        const TestExecutionSummary testSummary = full_output::CreateTestSummary(differencePosition, expectedOutput, output);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "\
Expected (context):\n\
SPC  v    e    r    r    y    \n\
                              \n\
0x20 0x76 0x65 0x72 0x72 0x79 \n\
Got (context):\n\
SPC  v    e    r    r    y    SPC  l    o    n    g    SPC  t    \n\
                              ^                                  \n\
0x20 0x76 0x65 0x72 0x72 0x79 0x20 0x6c 0x6f 0x6e 0x67 0x20 0x74"));
    }

}

namespace partial_output
{

TestExecutionSummary
CreateTestSummary(const std::string_view expectedPartialOutput)
{
    auto cause = expectation::validation::PartialOutputCause{expectedPartialOutput};

    return {
        Verdict::FAIL,
        {cause}
    };
}

void
CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(const std::string &console_log,
                                       const std::string &char_text,
                                       const std::string &char_code)
{
    CHECK(contain(console_log, "Expected (context):\n"
                               + char_text + "  a    \n"
                               + char_code + " 0x61"));
}

}

TEST_GROUP("Partial Output Cause logging")
{

    UNIT_TEST("Should contain failure desctiption")
    {
        const std::string expectedPartialOutput = "some verry";
        const TestExecutionSummary testSummary = partial_output::CreateTestSummary(expectedPartialOutput);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "Text not found in output."));
    }

    UNIT_TEST("Should contain text and hex context up to six bytes when expected partial text is longer")
    {
        const std::string expectedPartialOutput = "some long text";
        const TestExecutionSummary testSummary = partial_output::CreateTestSummary(expectedPartialOutput);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "\
Expected (context):\n\
s    o    m    e    SPC  l    o    \n\
0x73 0x6f 0x6d 0x65 0x20 0x6c 0x6f"));
    }

    UNIT_TEST("Should contain text and hex context up to five bytes when expected partial text have five bytes")
    {
        const std::string expectedPartialOutput = "Some ";
        const TestExecutionSummary testSummary = partial_output::CreateTestSummary(expectedPartialOutput);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "\
Expected (context):\n\
S    o    m    e    SPC  \n\
0x53 0x6f 0x6d 0x65 0x20"));
    }

    UNIT_TEST("Should contain one byte of text and hex context up when expected partial text have one bytetes")
    {
        const std::string expectedPartialOutput = "S";
        const TestExecutionSummary testSummary = partial_output::CreateTestSummary(expectedPartialOutput);

        const auto console_log = ExecuteSut(testSummary);

        CHECK(contain(console_log, "\
Expected (context):\n\
S    \n\
0x53"));
    }

    UNIT_TEST("Should contain SPC instead of space character")
    {
        const std::string expectedPartialOutput = " a";
        const TestExecutionSummary testSummary = partial_output::CreateTestSummary(expectedPartialOutput);

        const auto console_log = ExecuteSut(testSummary);

        partial_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "SPC", "0x20");
    }

    UNIT_TEST("Should contain TAB instead of horizontal tab character")
    {
        const std::string expectedPartialOutput = "\ta";
        const TestExecutionSummary testSummary = partial_output::CreateTestSummary(expectedPartialOutput);

        const auto console_log = ExecuteSut(testSummary);

        partial_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "TAB", "0x09");
    }

    UNIT_TEST("Should contain CR instead of carriage return character")
    {
        const std::string expectedPartialOutput = "\ra";
        const TestExecutionSummary testSummary = partial_output::CreateTestSummary(expectedPartialOutput);

        const auto console_log = ExecuteSut(testSummary);

        partial_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "CR ", "0x0d");
    }

    UNIT_TEST("Should contain LF instead of line feed character")
    {
        const std::string expectedPartialOutput = "\na";
        const TestExecutionSummary testSummary = partial_output::CreateTestSummary(expectedPartialOutput);

        const auto console_log = ExecuteSut(testSummary);

        partial_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "LF ", "0x0a");
    }

    UNIT_TEST("Should contain VT instead of vertical tab character")
    {
        const std::string expectedPartialOutput = "\va";
        const TestExecutionSummary testSummary = partial_output::CreateTestSummary(expectedPartialOutput);

        const auto console_log = ExecuteSut(testSummary);

        partial_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "VT ", "0x0b");
    }

    UNIT_TEST("Should contain FF instead of feed character")
    {
        const std::string expectedPartialOutput = "\fa";
        const TestExecutionSummary testSummary = partial_output::CreateTestSummary(expectedPartialOutput);

        const auto console_log = ExecuteSut(testSummary);

        partial_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "FF ", "0x0c");
    }

    UNIT_TEST("Should contain NP instead of feed character")
    {
        constexpr char nonPrintableCharacter = 0x1e;
        constexpr char nonPrintableString[] {nonPrintableCharacter, 'a', 0x00};
        const std::string expectedPartialOutput = nonPrintableString;
        const TestExecutionSummary testSummary = partial_output::CreateTestSummary(expectedPartialOutput);

        const auto console_log = ExecuteSut(testSummary);

        partial_output::CHECK_CONTAIN_CONTEXT_WITH_CUSTOM_CHAR(console_log, "NP ", "0x1e");
    }

}

}
