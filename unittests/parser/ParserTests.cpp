/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
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
    void CheckOutput(const TestData &data, const std::string &expectedOutput)
    {
        CHECK(data.expectations.size() == 1);
        expectation::Expectation *expectation = data.expectations.at(0).get();
        expectation::FullOutputExpectation *fullOutput = dynamic_cast<expectation::FullOutputExpectation*>(expectation);
        CHECK(fullOutput != nullptr);
        CHECK(fullOutput->GetContent() == expectedOutput);
    }
}


TEST_GROUP("Overall")
{
    UNIT_TEST("Should parse correct tokens flow")
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
        CheckOutput(data, "example output");
    }

    UNIT_TEST("Multiple 'parse()' call should return same test data")
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

        {
            const TestData &data = sut.parse();
            CHECK(data.input == "example input");
            CheckOutput(data, "example output");
        }

        {
            const TestData &data = sut.parse();
            CHECK(data.input == "example input");
            CheckOutput(data, "example output");
        }

        {
            const TestData &data = sut.parse();
            CHECK(data.input == "example input");
            CheckOutput(data, "example output");
        }
    }

    UNIT_TEST("Should throw exception on additional keyword at the end")
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

        CHECK_THROWS_AS(sut.parse(), exception::UnexpectedTokenException);
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

        CHECK_THROWS_AS(sut.parse(), exception::UnexpectedTokenException);
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
}


TEST_GROUP("'INPUT' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
    }

    UNIT_TEST("Should throw exception when lexer returns token other than 'INPUT' keyword")
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
}


TEST_GROUP("'EXPECT' state")
{
    UNIT_TEST("Should throw exception when lexer doesn't have more tokens")
    {
        LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                        lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                        lexer::Token{lexer::TokenKind::TEXT, "some text"}
        };
        Parser<LexerFake> sut(lexer);

        CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
    }

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
}


TEST_GROUP("'OUTPUT' state")
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

    UNIT_TEST("Should throw exception when lexer returns token other than 'OUTPUT'")
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
}

}
