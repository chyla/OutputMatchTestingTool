/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/parser/Parser.hpp"
#include "unittests/lexer/LexerFake.hpp"

#include "doctest/doctest/doctest.h"


namespace omtt::parser
{

using lexer::LexerFake;

TEST_CASE("Should parse correct tokens flow")
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
    CHECK(data.expectedOutput == "example output");
}

TEST_CASE("Multiple 'parse()' call should return same test data")
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
        CHECK(data.expectedOutput == "example output");
    }

    {
        const TestData &data = sut.parse();
        CHECK(data.input == "example input");
        CHECK(data.expectedOutput == "example output");
    }

    {
        const TestData &data = sut.parse();
        CHECK(data.input == "example input");
        CHECK(data.expectedOutput == "example output");
    }
}

TEST_CASE("In 'RUN' state  - Should throw exception when lexer doesn't have more tokens")
{
    LexerFake lexer{};
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
}

TEST_CASE("In 'RUN' state  - Should throw exception when lexer returns token other then 'RUN' keyword")
{
    LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "WITH"}
    };
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
}

TEST_CASE("In 'RUN' state  - Should throw exception when lexer returns 'RUN' text token")
{
    LexerFake lexer{lexer::Token{lexer::TokenKind::TEXT, "RUN"}
    };
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
}

TEST_CASE("In 'WITH' state  - Should throw exception when lexer doesn't have more tokens")
{
    LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"}
    };
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
}

TEST_CASE("In 'WITH' state  - Should throw exception when lexer returns token other then 'WITH' keyword")
{
    LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                  lexer::Token{lexer::TokenKind::KEYWORD, "RUN"}
    };
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
}

TEST_CASE("In 'WITH' state  - Should throw exception when lexer returns 'WITH' text token")
{
    LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                  lexer::Token{lexer::TokenKind::TEXT, "WITH"}
    };
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
}

TEST_CASE("In 'INPUT' state  - Should throw exception when lexer doesn't have more tokens")
{
    LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "WITH"}
    };
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
}

TEST_CASE("In 'INPUT' state  - Should throw exception when lexer returns token other then 'INPUT' keyword")
{
    LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"}
    };
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
}

TEST_CASE("In 'INPUT' state  - Should throw exception when lexer returns 'INPUT' text token")
{
    LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                    lexer::Token{lexer::TokenKind::TEXT, "INPUT"}
    };
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
}

TEST_CASE("In 'TEXT_INPUT' state  - Should throw exception when lexer doesn't have more tokens")
{
    LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"}
    };
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::MissingTextException);
}

TEST_CASE("In 'TEXT_INPUT' state  - Should throw exception when lexer returns token other then text token")
{
    LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "EXPECT"}
    };
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::UnexpectedKeywordException);
}

TEST_CASE("In 'EXPECT' state  - Should throw exception when lexer doesn't have more tokens")
{
    LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                    lexer::Token{lexer::TokenKind::TEXT, "some text"}
    };
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::MissingKeywordException);
}

TEST_CASE("In 'EXPECT' state  - Should throw exception when lexer returns token other then 'EXPECT'")
{
    LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                    lexer::Token{lexer::TokenKind::TEXT, "some text"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "WITH"}};
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
}

TEST_CASE("In 'EXPECT' state  - Should throw exception when lexer returns 'EXPECT' text token")
{
    LexerFake lexer{lexer::Token{lexer::TokenKind::KEYWORD, "RUN"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "WITH"},
                    lexer::Token{lexer::TokenKind::KEYWORD, "INPUT"},
                    lexer::Token{lexer::TokenKind::TEXT, "some text"},
                    lexer::Token{lexer::TokenKind::TEXT, "EXPECT"}};
    Parser<LexerFake> sut(lexer);

    CHECK_THROWS_AS(sut.parse(), exception::WrongTokenException);
}

TEST_CASE("In 'OUTPUT' state  - Should throw exception when lexer doesn't have more tokens")
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

TEST_CASE("In 'OUTPUT' state  - Should throw exception when lexer returns token other then 'OUTPUT'")
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

TEST_CASE("In 'OUTPUT' state  - Should throw exception when lexer returns 'OUTPUT' text token")
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

TEST_CASE("In 'TEXT_OUTPUT' state  - Should throw exception when lexer doesn't have more tokens")
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

TEST_CASE("In 'TEXT_OUTPUT' state  - Should throw exception when lexer returns token other then text token")
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

TEST_CASE("Should throw exception on additional keyword at the end")
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

TEST_CASE("Should throw exception on additional text at the end")
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
