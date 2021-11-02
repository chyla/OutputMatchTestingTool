/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/lexer/Token.hpp"
#include "headers/lexer/detail/State.hpp"

#include <optional>
#include <string>


namespace omtt::lexer
{

class Lexer {
public:
    explicit                              Lexer(const std::string &inputBuffer);
    virtual                               ~Lexer() = default;

             std::optional<const Token>   FindNextToken();

private:
    using PositionInBuffer = std::string::size_type;

    enum class ReadingKeywordsStateOptions {
        NONE,
        MOVE_TO_READING_LINES_STATE_AFTER_INPUT_OUTPUT
    };

private:
             std::optional<const Token>   _HandleReadingKeywordsState(Lexer::ReadingKeywordsStateOptions = Lexer::ReadingKeywordsStateOptions::NONE);
             std::optional<const Token>   _HandleReadingLinesUpToExpectState();
             std::optional<const Token>   _HandleReadingLinesUpToEofState();
             std::optional<const Token>   _HandleReadingInteger();

    inline   void                         _SwitchStateTo(const detail::State newState);

    const    std::string_view             _ReadNextWord();
             PositionInBuffer             _FindNextLetterPosition(const PositionInBuffer begin) const;
             PositionInBuffer             _FindNextWhiteCharPosition(const PositionInBuffer begin) const;
    inline   void                         _ConsumeWhiteCharactersWithoutNewLine();
    inline   void                         _ConsumeNewLineCharacter();
             std::optional<const Token>   _ConsumeAndGetTokenWithText(const Lexer::PositionInBuffer begin, const Lexer::PositionInBuffer end);

private:
    const    std::string &                fInputBuffer;
             std::string::size_type       fCurrentPosition;
             detail::State                fCurrentState;
};

}  // omtt::lexer
