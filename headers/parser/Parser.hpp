/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once


#include "headers/TestData.hpp"
#include "headers/lexer/Token.hpp"

#include "headers/parser/exception/MissingKeywordException.hpp"
#include "headers/parser/exception/WrongTokenException.hpp"
#include "headers/parser/exception/MissingTextException.hpp"
#include "headers/parser/exception/UnexpectedKeywordException.hpp"
#include "headers/parser/exception/UnexpectedTokenException.hpp"

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

    const TestData &
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
                case State::INPUT:
                    _HandleInputState();
                    break;
                case State::TEXT_INPUT:
                    _HandleTextInputState();
                    break;
                case State::EXPECT:
                    _HandleExpectState();
                    break;
                case State::OUTPUT:
                    _HandleOutputState();
                    break;
                case State::TEXT_OUTPUT:
                    _HandleTextOutputState();
                    break;
                case State::FINISHING:
                    _HandleFinishingState();
                    break;
                case State::DONE:
                  return fTestData;
            }
        }
    }

private:
      enum class State {
        RUN,
        WITH,
        INPUT,
        TEXT_INPUT,
        EXPECT,
        OUTPUT,
        TEXT_OUTPUT,
        FINISHING,
        DONE
    };

private:
    void
    _HandleRunState()
    {
        _ExpectKeywordAndSwitchToState("RUN", State::WITH);
    }

    void
    _HandleWithState()
    {
        _ExpectKeywordAndSwitchToState("WITH", State::INPUT);
    }

    void
    _HandleInputState()
    {
        _ExpectKeywordAndSwitchToState("INPUT", State::TEXT_INPUT);
    }

    void
    _HandleTextInputState()
    {
        auto token = fLexer.FindNextToken();

        _ThrowMissingTextWhenTokenNotPresent(token);
        _ThrowWhenKeyword(*token);

        fTestData.input = token->value;
        fCurrentState = State::EXPECT;
    }

    void
    _HandleExpectState()
    {
        _ExpectKeywordAndSwitchToState("EXPECT", State::OUTPUT);
    }

    void
    _HandleOutputState()
    {
        _ExpectKeywordAndSwitchToState("OUTPUT", State::TEXT_OUTPUT);
    }

    void
    _HandleTextOutputState()
    {
        auto token = fLexer.FindNextToken();

        _ThrowMissingTextWhenTokenNotPresent(token);
        _ThrowWhenKeyword(*token);

        fTestData.expectedOutput = token->value;
        fCurrentState = State::FINISHING;
    }

    void
    _HandleFinishingState()
    {
        auto token = fLexer.FindNextToken();

        _ThrowWhenTokenIsPresent(token);

        fCurrentState = State::DONE;
    }

    static void
    _ThrowMissingTextWhenTokenNotPresent(std::optional<const lexer::Token> &given)
    {
        if (!given.has_value()) {
            throw exception::MissingTextException();
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

        _ThrowMissingKeywordWhenTokenNotPresent(expectedKeywordName, token);
        _ThrowWhenDifferentKindOrKeywordName(expectedKeywordName, *token);

        fCurrentState = state;
    }

    static void
    _ThrowMissingKeywordWhenTokenNotPresent(const std::string &expectedKeywordName, std::optional<const lexer::Token> &given)
    {
        if (!given.has_value()) {
            throw exception::MissingKeywordException(expectedKeywordName);
        }
    }

    static void
    _ThrowWhenDifferentKindOrKeywordName(const std::string &expectedValue, const lexer::Token &given)
    {
        constexpr auto expectedKind = lexer::TokenKind::KEYWORD;
        if (given.kind != expectedKind
            || given.value != expectedValue) {
            throw exception::WrongTokenException(expectedValue, expectedKind, given);
        }
    }

    static void
    _ThrowWhenTokenIsPresent(std::optional<const lexer::Token> &token)
    {
        if (token.has_value()) {
            throw exception::UnexpectedTokenException(*token);
        }
    }

private:
    Lexer &    fLexer;
    State      fCurrentState;
    TestData   fTestData;
};

}  // omtt::parser
