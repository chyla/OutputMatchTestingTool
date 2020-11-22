/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/lexer/Lexer.hpp"
#include "headers/lexer/exception/UnexpectedCharacterException.hpp"

#include "unittests/test_framework.hpp"


namespace omtt::lexer
{

namespace helper
{

void
check_has_no_more_tokens(Lexer &sut)
{
    auto token = sut.FindNextToken();
    CHECK(!token.has_value());
}

void
check_token_equality(const std::optional<const Token> &actual, const Token &expected)
{
    REQUIRE(actual.has_value());
    CHECK(actual->kind == expected.kind);
    CHECK(actual->value == expected.value);
}

void
test_one_token_with_buffer(const std::string &buffer,
                           const Token &expectedToken)
{
    Lexer sut(buffer);

    auto token = sut.FindNextToken();

    check_token_equality(token, expectedToken);
    check_has_no_more_tokens(sut);
}

void
test_two_tokens_with_buffer(const std::string &buffer,
                            const Token &expectedFirstToken,
                            const Token &expectedSecondToken)
{
    Lexer sut(buffer);

    auto token = sut.FindNextToken();
    auto secondToken = sut.FindNextToken();

    check_token_equality(token, expectedFirstToken);
    check_token_equality(secondToken, expectedSecondToken);
    check_has_no_more_tokens(sut);
}

} // helper


TEST_CASE("Should return empty optional on empty input")
{
    const std::string buffer = "";
    Lexer sut(buffer);

    helper::check_has_no_more_tokens(sut);
}

TEST_CASE("Should return keyword token with 'RUN' value")
{
    const std::string buffer = "RUN";
    const Token expectedToken {TokenKind::KEYWORD, "RUN"};
    helper::test_one_token_with_buffer(buffer, expectedToken);
}

TEST_CASE("Should return keyword token with 'WITH' value")
{
    const std::string buffer = "WITH";
    const Token expectedToken {TokenKind::KEYWORD, "WITH"};
    helper::test_one_token_with_buffer(buffer, expectedToken);
}

TEST_CASE("Should return keyword token with 'INPUT' value")
{
    const std::string buffer = "INPUT";
    Lexer sut(buffer);

    auto token = sut.FindNextToken();

    helper::check_token_equality(token, {TokenKind::KEYWORD, "INPUT"});
}

TEST_CASE("Should return keyword token with 'EXPECT' value")
{
    const std::string buffer = "EXPECT";
    const Token expectedToken {TokenKind::KEYWORD, "EXPECT"};
    helper::test_one_token_with_buffer(buffer, expectedToken);
}

TEST_CASE("Should return keyword token with 'OUTPUT' value")
{
    const std::string buffer = "OUTPUT";
    Lexer sut(buffer);

    auto token = sut.FindNextToken();

    helper::check_token_equality(token, {TokenKind::KEYWORD, "OUTPUT"});
}

TEST_CASE("Should return keyword token with 'EXIT' value")
{
    const std::string buffer = "EXIT";
    const Token expectedToken {TokenKind::KEYWORD, "EXIT"};
    helper::test_one_token_with_buffer(buffer, expectedToken);
}

TEST_CASE("Should return keyword token with 'IN' value")
{
    const std::string buffer = "IN";
    const Token expectedToken {TokenKind::KEYWORD, "IN"};
    helper::test_one_token_with_buffer(buffer, expectedToken);
}

TEST_CASE("Should return keyword token with 'IN' value even when there are spaces at the begining of the input")
{
    const std::string buffer = "  IN";
    const Token expectedToken {TokenKind::KEYWORD, "IN"};
    helper::test_one_token_with_buffer(buffer, expectedToken);
}

TEST_CASE("Should return keyword token with 'IN' value even when there are spaces at the end of the input")
{
    const std::string buffer = "IN  ";
    const Token expectedToken {TokenKind::KEYWORD, "IN"};
    helper::test_one_token_with_buffer(buffer, expectedToken);
}

TEST_CASE("Should return keyword token with 'IN' value even when there are tabs at the begining of the input")
{
    const std::string buffer = "\t\tIN";
    const Token expectedToken {TokenKind::KEYWORD, "IN"};
    helper::test_one_token_with_buffer(buffer, expectedToken);
}

TEST_CASE("Should return keyword token with 'IN' value even when there are tabs at the end of the input")
{
    const std::string buffer = "IN\t\t";
    const Token expectedToken {TokenKind::KEYWORD, "IN"};
    helper::test_one_token_with_buffer(buffer, expectedToken);
}

TEST_CASE("Should return keyword token with 'RUN' value even when there are spaces at the begining of the input")
{
    const std::string buffer = "    RUN";
    const Token expectedToken {TokenKind::KEYWORD, "RUN"};
    helper::test_one_token_with_buffer(buffer, expectedToken);
}

TEST_CASE("Should return two keyword tokens for two keywords")
{
    const std::string buffer = "RUN WITH";
    const Token expectedFirstToken {TokenKind::KEYWORD, "RUN"};
    const Token expectedSecondToken {TokenKind::KEYWORD, "WITH"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("Should return two keyword tokens for two keywords which are separated with multiple spaces")
{
    const std::string buffer = "RUN    WITH";
    const Token expectedFirstToken {TokenKind::KEYWORD, "RUN"};
    const Token expectedSecondToken {TokenKind::KEYWORD, "WITH"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("Should return two keyword tokens for two keywords which are separated with multiple newline characters")
{
    const std::string buffer = "RUN\n\n\nWITH";
    const Token expectedFirstToken {TokenKind::KEYWORD, "RUN"};
    const Token expectedSecondToken {TokenKind::KEYWORD, "WITH"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("Should return text token for any text in buffer")
{
    const std::string buffer = "some text";
    const Token expectedFirstToken {TokenKind::TEXT, "some"};
    const Token expectedSecondToken {TokenKind::TEXT, "text"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("After the 'INPUT' keyword should return next line as text token when there is no 'EXPECT' keyword in next line")
{
    const std::string buffer = "INPUT\nsome input";
    const Token expectedFirstToken {TokenKind::KEYWORD, "INPUT"};
    const Token expectedSecondToken {TokenKind::TEXT, "some input"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("After the 'INPUT' keyword should ignore spaces and tabs up to the newline character")
{
    const std::string buffer = "INPUT \t  \t \n  some input";
    const Token expectedFirstToken {TokenKind::KEYWORD, "INPUT"};
    const Token expectedSecondToken {TokenKind::TEXT, "  some input"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("Should throw exception when characters other than spaces and tabs are found after the 'INPUT' keyword ")
{
    const std::string buffer = "INPUT other text\n  some input";
    Lexer sut(buffer);

    CHECK_THROWS_AS(sut.FindNextToken(), exception::UnexpectedCharacterException);
}

TEST_CASE("After the 'INPUT' keyword should return empty text token when there is no more text in buffer")
{
    const std::string buffer = "INPUT";
    const Token expectedFirstToken {TokenKind::KEYWORD, "INPUT"};
    const Token expectedSecondToken {TokenKind::TEXT, ""};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("After the 'INPUT' keyword should return all next lines as text token up to end of buffer when there is no line begining with 'EXPECT' keyword")
{
    const std::string buffer = "INPUT\nsome input\nother line\none more line";
    const Token expectedFirstToken {TokenKind::KEYWORD, "INPUT"};
    const Token expectedSecondToken {TokenKind::TEXT, "some input\nother line\none more line"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("After the 'INPUT' keyword should return all next lines as text token and ignore other keywords in this text")
{
    const std::string buffer = "INPUT\nsome EXPECT\n RUN \none more line";
    const Token expectedFirstToken {TokenKind::KEYWORD, "INPUT"};
    const Token expectedSecondToken {TokenKind::TEXT, "some EXPECT\n RUN \none more line"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("After the 'INPUT' keyword should ignore 'EXPECT' keyword when line starts with space")
{
    const std::string buffer = "INPUT\n EXPECT";
    const Token expectedFirstToken {TokenKind::KEYWORD, "INPUT"};
    const Token expectedSecondToken {TokenKind::TEXT, " EXPECT"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("After the 'INPUT' keyword should ignore 'EXPECT' keyword when line starts with tab")
{
    const std::string buffer = "INPUT\n\tEXPECT";
    const Token expectedFirstToken {TokenKind::KEYWORD, "INPUT"};
    const Token expectedSecondToken {TokenKind::TEXT, "\tEXPECT"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("After the 'INPUT' keyword should return empty text token when the next line is begining with 'EXPECT' keyword")
{
    const std::string buffer = "INPUT\nEXPECT";
    Lexer sut(buffer);

    auto token = sut.FindNextToken();
    auto secondToken = sut.FindNextToken();
    auto thirdToken = sut.FindNextToken();

    helper::check_token_equality(token, {TokenKind::KEYWORD, "INPUT"});
    helper::check_token_equality(secondToken, {TokenKind::TEXT, ""});
    helper::check_token_equality(thirdToken, {TokenKind::KEYWORD, "EXPECT"});
    helper::check_has_no_more_tokens(sut);
}

TEST_CASE("After the 'INPUT' keyword should return all next lines as text token up to line begining with 'EXPECT' keyword")
{
    const std::string buffer = "INPUT\nsome input\nother line\none more line\nEXPECT";
    Lexer sut(buffer);

    auto token = sut.FindNextToken();
    auto secondToken = sut.FindNextToken();
    auto thirdToken = sut.FindNextToken();

    helper::check_token_equality(token, {TokenKind::KEYWORD,
                                         "INPUT"});
    helper::check_token_equality(secondToken, {TokenKind::TEXT,
                                               "some input\nother line\none more line"});
    helper::check_token_equality(thirdToken, {TokenKind::KEYWORD,
                                              "EXPECT"});
    helper::check_has_no_more_tokens(sut);
}

TEST_CASE("Should return proper text token when multiple 'INPUT' keywords are present")
{
    const std::string buffer = "INPUT\nsome text\nEXPECT INPUT\nother line\nEXPECT";
    Lexer sut(buffer);

    auto token = sut.FindNextToken();
    auto secondToken = sut.FindNextToken();
    auto thirdToken = sut.FindNextToken();
    auto fourthToken = sut.FindNextToken();
    auto fifthToken = sut.FindNextToken();
    auto sixthToken = sut.FindNextToken();

    helper::check_token_equality(token, {TokenKind::KEYWORD, "INPUT"});
    helper::check_token_equality(secondToken, {TokenKind::TEXT, "some text"});
    helper::check_token_equality(thirdToken, {TokenKind::KEYWORD, "EXPECT"});
    helper::check_token_equality(fourthToken, {TokenKind::KEYWORD, "INPUT"});
    helper::check_token_equality(fifthToken, {TokenKind::TEXT, "other line"});
    helper::check_token_equality(sixthToken, {TokenKind::KEYWORD, "EXPECT"});
    helper::check_has_no_more_tokens(sut);
}

TEST_CASE("After the 'CODE' keyword should finish reading tokens when there is no more text in buffer")
{
    const std::string buffer = "CODE";
    const Token expectedToken {TokenKind::KEYWORD, "CODE"};
    helper::test_one_token_with_buffer(buffer, expectedToken);
}

TEST_CASE("After the 'CODE' keyword should return integer token with correct one digit value")
{
    const std::string buffer = "CODE 0";
    const Token expectedFirstToken {TokenKind::KEYWORD, "CODE"};
    const Token expectedSecondToken {TokenKind::INTEGER, "0"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("After the 'CODE' keyword should return integer token with correct more than one digit value")
{
    const std::string buffer = "CODE 3152";
    const Token expectedFirstToken {TokenKind::KEYWORD, "CODE"};
    const Token expectedSecondToken {TokenKind::INTEGER, "3152"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("After the 'CODE' keyword should return integer token with correct value when surrounded with white characters")
{
    const std::string buffer = "CODE    \t 3152  \t ";
    const Token expectedFirstToken {TokenKind::KEYWORD, "CODE"};
    const Token expectedSecondToken {TokenKind::INTEGER, "3152"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("After the 'CODE' keyword should throw exception when number has characters other than digits")
{
    const std::string buffer = "CODE    \t 3d1d5s2  \t ";
    const Token expectedFirstToken {TokenKind::KEYWORD, "CODE"};
    Lexer sut(buffer);

    auto token = sut.FindNextToken();
    helper::check_token_equality(token, expectedFirstToken);

    CHECK_THROWS_AS(sut.FindNextToken(), exception::UnexpectedCharacterException);
}

TEST_CASE("After the 'CODE' keyword should return integer and parse next keyword")
{
    const std::string buffer = "CODE 3152 EXPECT";
    Lexer sut(buffer);

    auto token = sut.FindNextToken();
    auto secondToken = sut.FindNextToken();
    auto thirdToken = sut.FindNextToken();

    helper::check_token_equality(token, {TokenKind::KEYWORD,
                                         "CODE"});
    helper::check_token_equality(secondToken, {TokenKind::INTEGER,
                                               "3152"});
    helper::check_token_equality(thirdToken, {TokenKind::KEYWORD,
                                              "EXPECT"});
    helper::check_has_no_more_tokens(sut);
}

TEST_CASE("After the 'OUTPUT' keyword should return empty text token when there is no more text in buffer")
{
    const std::string buffer = "OUTPUT";
    const Token expectedFirstToken {TokenKind::KEYWORD, "OUTPUT"};
    const Token expectedSecondToken {TokenKind::TEXT, ""};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("After the 'OUTPUT' keyword should return next line as text token")
{
    const std::string buffer = "OUTPUT\nsome output";
    const Token expectedFirstToken {TokenKind::KEYWORD, "OUTPUT"};
    const Token expectedSecondToken {TokenKind::TEXT, "some output"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("After the 'OUTPUT' keyword should ignore spaces and tabs up to the newline character")
{
    const std::string buffer = "OUTPUT \t  \t \n  some output";
    const Token expectedFirstToken {TokenKind::KEYWORD, "OUTPUT"};
    const Token expectedSecondToken {TokenKind::TEXT, "  some output"};
    helper::test_two_tokens_with_buffer(buffer, expectedFirstToken, expectedSecondToken);
}

TEST_CASE("After the 'OUTPUT' keyword should return all next lines as text token up to the end of file")
{
    const std::string buffer = "OUTPUT\nsome input\nother line\nsome text";
    Lexer sut(buffer);

    auto token = sut.FindNextToken();
    auto secondToken = sut.FindNextToken();

    helper::check_token_equality(token, {TokenKind::KEYWORD,
                                         "OUTPUT"});
    helper::check_token_equality(secondToken, {TokenKind::TEXT,
                                               "some input\nother line\nsome text"});
    helper::check_has_no_more_tokens(sut);
}

TEST_CASE("After the 'OUTPUT' keyword should return all next lines as text token up to the next EXPECT keyword in new line")
{
    const std::string buffer = "OUTPUT\nsome input\nother line\nsome text\nEXPECT";
    Lexer sut(buffer);

    auto token = sut.FindNextToken();
    auto secondToken = sut.FindNextToken();
    auto thirdToken = sut.FindNextToken();

    helper::check_token_equality(token, {TokenKind::KEYWORD,
                                         "OUTPUT"});
    helper::check_token_equality(secondToken, {TokenKind::TEXT,
                                               "some input\nother line\nsome text"});
    helper::check_token_equality(thirdToken, {TokenKind::KEYWORD,
                                              "EXPECT"});
    helper::check_has_no_more_tokens(sut);
}

TEST_CASE("After the 'OUTPUT' keyword should return all next lines as text token up to the end of file even when there are other keywords (except 'EXPECT') in these lines")
{
    const std::string buffer = "OUTPUT\nsome input\nother line\noline with keyword RUN\nWITH";
    Lexer sut(buffer);

    auto token = sut.FindNextToken();
    auto secondToken = sut.FindNextToken();

    helper::check_token_equality(token, {TokenKind::KEYWORD,
                                         "OUTPUT"});
    helper::check_token_equality(secondToken, {TokenKind::TEXT,
                                               "some input\nother line\noline with keyword RUN\nWITH"});
    helper::check_has_no_more_tokens(sut);
}

TEST_CASE("After handling the whole input should return empty optional when there are multiple check for next token")
{
    const std::string buffer = "RUN WITH";
    Lexer sut(buffer);

    auto token = sut.FindNextToken();
    auto secondToken = sut.FindNextToken();

    helper::check_token_equality(token, {TokenKind::KEYWORD,
                                         "RUN"});
    helper::check_token_equality(secondToken, {TokenKind::KEYWORD,
                                               "WITH"});
    helper::check_has_no_more_tokens(sut);
    helper::check_has_no_more_tokens(sut);
    helper::check_has_no_more_tokens(sut);
    helper::check_has_no_more_tokens(sut);
    helper::check_has_no_more_tokens(sut);
}

TEST_CASE("Should read 'IN' keyword when separated with new lines")
{
    const std::string buffer = "EXPECT\nIN\nOUTPUT";
    Lexer sut(buffer);

    auto token = sut.FindNextToken();
    auto secondToken = sut.FindNextToken();
    auto thirdToken = sut.FindNextToken();
    auto fourthToken = sut.FindNextToken();

    helper::check_token_equality(token, {TokenKind::KEYWORD, "EXPECT"});
    helper::check_token_equality(secondToken, {TokenKind::KEYWORD, "IN"});
    helper::check_token_equality(thirdToken, {TokenKind::KEYWORD, "OUTPUT"});
    helper::check_token_equality(fourthToken, {TokenKind::TEXT, ""});
    helper::check_has_no_more_tokens(sut);
}

}  // omtt::lexer
