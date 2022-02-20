/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once


#include "headers/TestData.hpp"
#include "headers/lexer/Token.hpp"

#include "headers/expectation/EmptyOutputExpectation.hpp"
#include "headers/expectation/FullOutputExpectation.hpp"
#include "headers/expectation/PartialOutputExpectation.hpp"
#include "headers/expectation/ExitCodeExpectation.hpp"
#include "headers/expectation/SuccessfulExitExpectation.hpp"

#include "headers/parser/exception/MissingKeywordException.hpp"
#include "headers/parser/exception/WrongTokenException.hpp"
#include "headers/parser/exception/MissingTextException.hpp"
#include "headers/parser/exception/MissingIntegerException.hpp"
#include "headers/parser/exception/UnexpectedKeywordException.hpp"

#include <algorithm>
#include <initializer_list>
#include <optional>
#include <string>


namespace omtt::parser
{

template<class Lexer>
class Parser {
public:
    explicit Parser(Lexer &lexer)
        :
        fLexer(lexer),
        fCurrentState(State::RUN)
    {
    }

    TestData &&
    parse()
    {
        while (true) {
            switch (fCurrentState) {
                case State::RUN:
                    _HandleRunState();
                    break;
                case State::WITH:
                    _HandleWithState();
                    break;
                case State::EMPTY_OR_INPUT:
                    _HandleEmptyOrInputState();
                    break;
                case State::EMPTY_INPUT:
                    _HandleEmptyInputState();
                    break;
                case State::TEXT_INPUT:
                    _HandleTextInputState();
                    break;
                case State::EXPECT_OR_FINISH:
                    _HandleExpectOrFinishState();
                    break;
                case State::OUTPUT_OR_EXIT_OR_IN:
                    _HandleEmptyOrOutputOrExitOrInState();
                    break;
                case State::IN_OUTPUT:
                    _HandleInOutputState();
                    break;
                case State::EMPTY_OUTPUT:
                    _HandleEmptyOutputState();
                    break;
                case State::CODE_OR_WITH:
                    _HandleCodeOrWithState();
                    break;
                case State::CODE_NUMBER:
                    _HandleCodeNumberState();
                    break;
                case State::EXIT_WITH_SUCCESS:
                    _HandleExitWithSuccessState();
                    break;
                case State::TEXT_OUTPUT:
                    _HandleTextOutputState();
                    break;
                case State::TEXT_IN_OUTPUT:
                    _HandleTextInOutputState();
                    break;
                case State::DONE:
                  return std::move(fTestData);
            }
        }
    }

private:
      enum class State {
        RUN,
        WITH,
        EMPTY_OR_INPUT,
        EMPTY_INPUT,
        TEXT_INPUT,
        EXPECT_OR_FINISH,
        OUTPUT_OR_EXIT_OR_IN,
        IN_OUTPUT,
        EMPTY_OUTPUT,
        CODE_OR_WITH,
        CODE_NUMBER,
        EXIT_WITH_SUCCESS,
        TEXT_OUTPUT,
        TEXT_IN_OUTPUT,
        DONE
    };

private:
    void
    _HandleRunState()
    {
        _IgnoreCommentExpectKeywordAndSwitchToState("RUN", State::WITH);
    }

    void
    _HandleWithState()
    {
        _ExpectKeywordAndSwitchToState("WITH", State::EMPTY_OR_INPUT);
    }

    void
    _HandleEmptyOrInputState()
    {
        auto token = fLexer.FindNextToken();

        _ThrowMissingKeywordWhenTokenNotPresent({"EMPTY", "INPUT"}, token);

        if (token->kind == lexer::TokenKind::KEYWORD
                 && token->value == "INPUT") {
            fCurrentState = State::TEXT_INPUT;
        }
        else if (token->kind == lexer::TokenKind::KEYWORD
                 && token->value == "EMPTY") {
            fCurrentState = State::EMPTY_INPUT;
        }
        else {
            _ThrowWhenNotKeywordOrHasDifferrentName({"EMPTY", "INPUT"}, *token);
        }
    }

    void
    _HandleEmptyInputState()
    {
        _ExpectKeywordAndSwitchToState("INPUT", State::EXPECT_OR_FINISH);
    }

    void
    _HandleTextInputState()
    {
        auto token = fLexer.FindNextToken();

        _ThrowMissingTextWhenTokenNotPresent(token);
        _ThrowWhenKeyword(*token);

        fTestData.input = token->value;
        fCurrentState = State::EXPECT_OR_FINISH;
    }

    void
    _HandleExpectOrFinishState()
    {
        auto token = fLexer.FindNextToken();

        if (token.has_value()) {
            _ThrowWhenNotKeywordOrHasDifferrentName({"EXPECT"}, *token);
            fCurrentState = State::OUTPUT_OR_EXIT_OR_IN;
        }
        else {
            fCurrentState = State::DONE;
        }
    }

    void
    _HandleEmptyOrOutputOrExitOrInState()
    {
        auto token = fLexer.FindNextToken();

        _ThrowMissingKeywordWhenTokenNotPresent({"EMPTY", "OUTPUT", "EXIT", "IN"}, token);

        if (token->kind == lexer::TokenKind::KEYWORD
            && token->value == "OUTPUT") {
            fCurrentState = State::TEXT_OUTPUT;
        }
        else if (token->kind == lexer::TokenKind::KEYWORD
                 && token->value == "EXIT") {
            fCurrentState = State::CODE_OR_WITH;
        }
        else if (token->kind == lexer::TokenKind::KEYWORD
                 && token->value == "IN") {
            fCurrentState = State::IN_OUTPUT;
        }
        else if (token->kind == lexer::TokenKind::KEYWORD
                 && token->value == "EMPTY") {
            fCurrentState = State::EMPTY_OUTPUT;
        }
        else {
            _ThrowWhenNotKeywordOrHasDifferrentName({"EMPTY", "OUTPUT", "EXIT", "IN"}, *token);
        }
    }

    void
    _HandleInOutputState()
    {
        _ExpectKeywordAndSwitchToState("OUTPUT", State::TEXT_IN_OUTPUT);
    }

    void
    _HandleEmptyOutputState()
    {
        _ExpectKeywordAndSwitchToState("OUTPUT", State::EXPECT_OR_FINISH);

        auto expectation = std::make_unique<expectation::EmptyOutputExpectation>();
        fTestData.expectations.emplace_back(std::move(expectation));
    }

    void
    _HandleCodeOrWithState()
    {
        auto token = fLexer.FindNextToken();

        _ThrowMissingKeywordWhenTokenNotPresent({"CODE", "WITH"}, token);

        if (token->kind == lexer::TokenKind::KEYWORD
                 && token->value == "CODE") {
            fCurrentState = State::CODE_NUMBER;
        }
        else if (token->kind == lexer::TokenKind::KEYWORD
                 && token->value == "WITH") {
            fCurrentState = State::EXIT_WITH_SUCCESS;
        }
        else {
            _ThrowWhenNotKeywordOrHasDifferrentName({"CODE", "WITH"}, *token);
        }
    }

    void
    _HandleCodeNumberState()
    {
        constexpr auto expectedTokenKind = lexer::TokenKind::INTEGER;

        auto token = fLexer.FindNextToken();

        _ThrowMissingIntegerWhenTokenNotPresent(token);

        if (token->kind == expectedTokenKind) {
            const auto expectedCode = std::stoi(std::string(token->value));
            auto expectation = std::make_unique<expectation::ExitCodeExpectation>(expectedCode);
            fTestData.expectations.emplace_back(std::move(expectation));

            fCurrentState = State::EXPECT_OR_FINISH;
        }
        else {
            throw exception::WrongTokenException({}, expectedTokenKind, *token);
        }
    }

    void
    _HandleExitWithSuccessState()
    {
        _ExpectKeywordAndSwitchToState("SUCCESS", State::EXPECT_OR_FINISH);

        auto expectation = std::make_unique<expectation::SuccessfulExitExpectation>();
        fTestData.expectations.emplace_back(std::move(expectation));
    }

    void
    _HandleTextOutputState()
    {
        auto token = fLexer.FindNextToken();

        _ThrowMissingTextWhenTokenNotPresent(token);
        _ThrowWhenKeyword(*token);

        auto expectation = std::make_unique<expectation::FullOutputExpectation>(token->value);
        fTestData.expectations.emplace_back(std::move(expectation));

        fCurrentState = State::EXPECT_OR_FINISH;
    }

    void
    _HandleTextInOutputState()
    {
        auto token = fLexer.FindNextToken();

        _ThrowMissingTextWhenTokenNotPresent(token);
        _ThrowWhenKeyword(*token);

        auto expectation = std::make_unique<expectation::PartialOutputExpectation>(token->value);
        fTestData.expectations.emplace_back(std::move(expectation));

        fCurrentState = State::EXPECT_OR_FINISH;
    }

    static void
    _ThrowMissingTextWhenTokenNotPresent(std::optional<const lexer::Token> &given)
    {
        if (!given.has_value()) {
            throw exception::MissingTextException();
        }
    }

    static void
    _ThrowMissingIntegerWhenTokenNotPresent(std::optional<const lexer::Token> &given)
    {
        if (!given.has_value()) {
            throw exception::MissingIntegerException();
        }
    }

    static void
    _ThrowWhenKeyword(const lexer::Token &given)
    {
        if (given.kind == lexer::TokenKind::KEYWORD) {
            throw exception::UnexpectedKeywordException(static_cast<std::string>(given.value));
        }
    }

    void
    _ExpectKeywordAndSwitchToState(const std::string &expectedKeywordName, const State state)
    {
        auto token = fLexer.FindNextToken();

        _ThrowMissingKeywordWhenTokenNotPresent({expectedKeywordName}, token);
        _ThrowWhenNotKeywordOrHasDifferrentName({expectedKeywordName}, *token);

        fCurrentState = state;
    }

    void
    _IgnoreCommentExpectKeywordAndSwitchToState(const std::string &expectedKeywordName, const State state)
    {
        auto token = _GetNextNonCommentToken();

        _ThrowMissingKeywordWhenTokenNotPresent({expectedKeywordName}, token);
        _ThrowWhenNotKeywordOrHasDifferrentName({expectedKeywordName}, *token);

        fCurrentState = state;
    }

    std::optional<const omtt::lexer::Token>
    _GetNextNonCommentToken()
    {
        while (true) {
            auto token = fLexer.FindNextToken();

            if (!token.has_value()
                || (token.has_value() && token->kind != omtt::lexer::TokenKind::COMMENT)) {
                    return token;
            }
        }
    }

    static void
    _ThrowMissingKeywordWhenTokenNotPresent(const std::initializer_list<const std::string> &expectedKeywordsNames,
                                            std::optional<const lexer::Token> &given)
    {
        if (!given.has_value()) {
            throw exception::MissingKeywordException(expectedKeywordsNames);
        }
    }

    static void
    _ThrowWhenNotKeywordOrHasDifferrentName(const std::initializer_list<const std::string> expectedNames,
                                            const lexer::Token &given)
    {
        constexpr auto expectedKind = lexer::TokenKind::KEYWORD;
        if (given.kind != expectedKind
            || !_ValueIn(given.value, expectedNames)) {
            throw exception::WrongTokenException(expectedNames, expectedKind, given);
        }
    }

    template<class Value, class Container>
    static bool
    _ValueIn(const Value &value, const Container &list)
    {
        return std::any_of(list.begin(), list.end(),
                           [&](const auto &item) {
                               return item == value;
                           });
    }

private:
    Lexer &    fLexer;
    State      fCurrentState;
    TestData   fTestData;
};

}  // omtt::parser
