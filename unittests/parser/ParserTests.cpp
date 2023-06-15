/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/parser/Parser.hpp"
#include "unittests/lexer/LexerFake.hpp"

#include "unittests/test_framework.hpp"


namespace omtt::parser
{

using lexer::LexerFake;

namespace
{
    void CheckNoExpectations(const TestData &data)
    {
        CHECK(data.expectations.size() == 0);
    }

    void CheckHasOneExpectation(const TestData &data)
    {
        CHECK(data.expectations.size() == 1);
    }

    void CheckHasTwoExpectations(const TestData &data)
    {
        CHECK(data.expectations.size() == 2);
    }

    void CheckOutput(const TestData &data, const std::string &expectedOutput, const int position = 0)
    {
        expectation::Expectation *expectation = data.expectations.at(position).get();
        expectation::FullOutputExpectation *fullOutput = dynamic_cast<expectation::FullOutputExpectation*>(expectation);
        CHECK(fullOutput != nullptr);
        CHECK(fullOutput->GetContent() == expectedOutput);
    }

    void CheckEmptyOutput(const TestData &data, const int position = 0)
    {
        expectation::Expectation *expectation = data.expectations.at(position).get();
        expectation::EmptyOutputExpectation *emptyOutput = dynamic_cast<expectation::EmptyOutputExpectation*>(expectation);
        CHECK(emptyOutput != nullptr);
    }

    void CheckPartialOutput(const TestData &data, const std::string &expectedOutput, const int position = 0)
    {
        expectation::Expectation *expectation = data.expectations.at(position).get();
        expectation::PartialOutputExpectation *fullOutput = dynamic_cast<expectation::PartialOutputExpectation*>(expectation);
        CHECK(fullOutput != nullptr);
        CHECK(fullOutput->GetContent() == expectedOutput);
    }

    void CheckExitCode(const TestData &data, const int expectedCode, const int position = 0)
    {
        expectation::Expectation *expectation = data.expectations.at(position).get();
        expectation::ExitCodeExpectation *exitCode = dynamic_cast<expectation::ExitCodeExpectation*>(expectation);
        CHECK(exitCode != nullptr);
        CHECK(exitCode->GetContent() == expectedCode);
    }

    void CheckExitWithSuccess(const TestData &data, const int position = 0)
    {
        expectation::Expectation *expectation = data.expectations.at(position).get();
        expectation::SuccessfulExitExpectation *exitExpectation = dynamic_cast<expectation::SuccessfulExitExpectation*>(expectation);
        CHECK(exitExpectation != nullptr);
    }

    void CheckExitWithFailure(const TestData &data, const int position = 0)
    {
        expectation::Expectation *expectation = data.expectations.at(position).get();
        expectation::FailureExitExpectation *exitExpectation = dynamic_cast<expectation::FailureExitExpectation*>(expectation);
        CHECK(exitExpectation != nullptr);
    }
}


TEST_GROUP("Overall")
{
    UNIT_TEST("Should ignore comments at the begining")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example input"}
        };
        Parser<LexerFake> sut(lexer);

        const TestData &data = sut.parse();

        CHECK(data.input == "example input");
        CheckNoExpectations(data);
    }

    UNIT_TEST("Should parse test without any EXPECT block")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example input"}
        };
        Parser<LexerFake> sut(lexer);

        const TestData &data = sut.parse();

        CHECK(data.input == "example input");
        CheckNoExpectations(data);
    }

    UNIT_TEST("Should parse correct full output match tokens flow")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example input"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example output"}
        };
        Parser<LexerFake> sut(lexer);

        const TestData &data = sut.parse();

        CHECK(data.input == "example input");
        CheckHasOneExpectation(data);
        CheckOutput(data, "example output");
    }

    UNIT_TEST("Should parse correct exit code tokens flow")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example input"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "CODE"},
                        lexer::Token{lexer::TokenKind::INTEGER, "13"}
        };
        Parser<LexerFake> sut(lexer);

        const TestData &data = sut.parse();

        CHECK(data.input == "example input");
        CheckHasOneExpectation(data);
        CheckExitCode(data, 13);
    }

    UNIT_TEST("Should parse correct exit with success tokens flow")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example input"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "SUCCESS"}
        };
        Parser<LexerFake> sut(lexer);

        const TestData &data = sut.parse();

        CheckHasOneExpectation(data);
        CheckExitWithSuccess(data);
    }

    UNIT_TEST("Should parse correct exit with failure tokens flow")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example input"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "FAILURE"}
        };
        Parser<LexerFake> sut(lexer);

        const TestData &data = sut.parse();

        CheckHasOneExpectation(data);
        CheckExitWithFailure(data);
    }

    UNIT_TEST("Should parse correct exit code and full match tokens flow")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example input"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "CODE"},
                        lexer::Token{lexer::TokenKind::INTEGER, "4561"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some output"}
        };
        Parser<LexerFake> sut(lexer);

        const TestData &data = sut.parse();

        CHECK(data.input == "example input");
        CheckHasTwoExpectations(data);

        constexpr int exitCodeExpectationPosition = 0;
        CheckExitCode(data, 4561, exitCodeExpectationPosition);

        constexpr int outputMatchPosition = 1;
        CheckOutput(data, "some output", outputMatchPosition);
    }

    UNIT_TEST("Should parse correct exit code and full match tokens flow when output match is before exit code match")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example input"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some output"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "CODE"},
                        lexer::Token{lexer::TokenKind::INTEGER, "4561"}
        };
        Parser<LexerFake> sut(lexer);

        const TestData &data = sut.parse();

        CHECK(data.input == "example input");
        CheckHasTwoExpectations(data);

        constexpr int outputMatchPosition = 0;
        CheckOutput(data, "some output", outputMatchPosition);

        constexpr int exitCodeExpectationPosition = 1;
        CheckExitCode(data, 4561, exitCodeExpectationPosition);
    }

    UNIT_TEST("Should parse correct partial output match tokens flow")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example input"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "IN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "partial output"}
        };
        Parser<LexerFake> sut(lexer);

        const TestData &data = sut.parse();

        CHECK(data.input == "example input");
        CheckHasOneExpectation(data);
        CheckPartialOutput(data, "partial output");
    }

    UNIT_TEST("Should parse correct empty output match tokens flow")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example input"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"}
        };
        Parser<LexerFake> sut(lexer);

        const TestData &data = sut.parse();

        CHECK(data.input == "example input");
        CheckHasOneExpectation(data);
        CheckEmptyOutput(data);
    }

    UNIT_TEST("Should parse correct empty input match tokens flow")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"}
        };
        Parser<LexerFake> sut(lexer);

        const TestData &data = sut.parse();

        CHECK(data.input == "");
    }

    UNIT_TEST("Should parse correct empty input with empty output match tokens flow")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"}
        };
        Parser<LexerFake> sut(lexer);

        const TestData &data = sut.parse();

        CHECK(data.input == "");
    }

    UNIT_TEST("Should throw exception on additional EXPECT keyword at the end")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example input"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example output"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
    }

    UNIT_TEST("Should throw exception on additional keyword other than EXPECT at the end")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example input"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example output"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception on additional text at the end")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example input"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "example output"},
                        lexer::Token{lexer::TokenKind::TEXT, "some additional text"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}


TEST_GROUP("'RUN' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{};
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
    }

    UNIT_TEST("Should throw exception when lexer returns token other than 'RUN' keyword")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "WITH"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'RUN' text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::TEXT, "RUN"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}


TEST_GROUP("'WITH' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
    }

    UNIT_TEST("Should throw exception when lexer returns token other than 'WITH' keyword")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "RUN"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'WITH' text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::TEXT, "WITH"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception on comments")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}


TEST_GROUP("'EMPTY_OR_INPUT' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
    }

    UNIT_TEST("Should throw exception when lexer returns token other than 'INPUT' or 'EMPTY' keyword")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'INPUT' text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::TEXT, "INPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'EMPTY' text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::TEXT, "EMPTY"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception on comments")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}

TEST_GROUP("'EMPTY_INPUT' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
    }

    UNIT_TEST("Should throw exception when lexer returns token other than 'INPUT' keyword")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'INPUT' text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::TEXT, "INPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception on comments")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}

TEST_GROUP("'TEXT_INPUT' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingTextException);
    }

    UNIT_TEST("Should throw exception when lexer returns token other than text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::UnexpectedKeywordException);
    }

    UNIT_TEST("Should throw exception on comments")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}


TEST_GROUP("'EXPECT_OR_FINISHING' state")
{
    UNIT_TEST("Should throw exception when lexer returns token other than 'EXPECT'")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"}};
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'EXPECT' text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::TEXT, "EXPECT"}};
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception on comments")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}


TEST_GROUP("'OUTPUT_OR_EXIT_OR_IN' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
    }

    UNIT_TEST("Should throw exception when lexer returns unexpected keyword token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'OUTPUT' text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::TEXT, "OUTPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'EXIT' text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::TEXT, "EXIT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'IN' text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::TEXT, "IN"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception on comments")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}


TEST_GROUP("'IN_OUTPUT' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "IN"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'OUTPUT' text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "IN"},
                        lexer::Token{lexer::TokenKind::TEXT, "OUTPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns unexpected keyword token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "IN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception on comments")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "IN"},
                        lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}

TEST_GROUP("'CODE_OR_WITH' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'CODE' text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::TEXT, "CODE"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'WITH' text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::TEXT, "WITH"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns unexpected keyword token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception on comments")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}


TEST_GROUP("'CODE_NUMBER' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't return exit code number")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "CODE"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingIntegerException);
    }

    UNIT_TEST("Should throw exception when lexer returns text token with number")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "CODE"},
                        lexer::Token{lexer::TokenKind::TEXT, "131"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns unexpected keyword token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "CODE"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns too big number")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "CODE"},
                        lexer::Token{lexer::TokenKind::INTEGER, "9999999999999999999999999999999999999"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), std::out_of_range);
    }

    UNIT_TEST("Should throw exception when lexer returns too small number")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "CODE"},
                        lexer::Token{lexer::TokenKind::INTEGER, "-9999999999999999999999999999999999"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), std::out_of_range);
    }

    UNIT_TEST("Should throw exception on comments")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "CODE"},
                        lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}


TEST_GROUP("'TEXT_OUTPUT' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingTextException);
    }

    UNIT_TEST("Should throw exception when lexer returns token other than text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::UnexpectedKeywordException);
    }

    UNIT_TEST("Should throw exception on comments")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"},
                        lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}


TEST_GROUP("'TEXT_IN_OUTPUT' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "IN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingTextException);
    }

    UNIT_TEST("Should throw exception when lexer returns token other than text token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "IN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::UnexpectedKeywordException);
    }

    UNIT_TEST("Should throw exception on comments")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "IN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "OUTPUT"},
                        lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}


TEST_GROUP("'EMPTY_OUTPUT' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
    }

    UNIT_TEST("Should throw exception when lexer returns token other than OUTPUT keyword")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'OUTPUT' TEXT token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::TEXT, "OUTPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception on comments")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}

TEST_GROUP("'EXIT_WITH_FAILURE_OR_SUCCESS' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
    }

    UNIT_TEST("Should throw exception when lexer returns token other than FAILURE or SUCCESS keyword")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'SUCCESS' TEXT token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::TEXT, "SUCCESS"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception when lexer returns 'FAILURE' TEXT token")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::TEXT, "FAILURE"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }

    UNIT_TEST("Should throw exception on comments")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EMPTY"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "EXIT"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::COMMENT, "/*"},
                        lexer::Token{lexer::TokenKind::COMMENT, "word"},
                        lexer::Token{lexer::TokenKind::COMMENT, "*/"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
    }
}

}
