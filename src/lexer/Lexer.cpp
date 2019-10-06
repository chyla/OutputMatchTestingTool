/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/lexer/Lexer.hpp"
#include "headers/lexer/exception/InvalidStateHandlingException.hpp"
#include "headers/lexer/exception/UnexpectedCharacterException.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string_view>


namespace omtt::lexer
{

using detail::State;


namespace
{

template<class UnaryPredicate>
Lexer::PositionInBuffer
find_next_position(const std::string &buffer,
                   Lexer::PositionInBuffer begin,
                   UnaryPredicate predicate)
{
    return std::distance(buffer.begin(),
                         std::find_if(buffer.begin() + begin,
                                      buffer.end(),
                                      predicate));
}

}

Lexer::Lexer(const std::string &inputBuffer)
    :
    fInputBuffer(inputBuffer)
{
    fCurrentPosition = 0;
    fCurrentState = State::READING_KEYWORDS;
}

std::optional<const Token>
Lexer::FindNextToken()
{
    switch (fCurrentState) {
        case State::READING_KEYWORDS:
            return _HandleReadingKeywordsState();

        case State::READING_LINES_UP_TO_EXPECT:
            return _HandleReadingLinesUpToExpectState();

        case State::READING_LINES_UP_TO_EOF:
            return _HandleReadingLinesUpToEofState();
    };

    throw exception::InvalidStateHandlingException(fCurrentState);
}

std::optional<const Token>
Lexer::_HandleReadingKeywordsState()
{
    const std::string_view word = _ReadNextWord();

    if (word == "INPUT") {
        _SwitchStateTo(State::READING_LINES_UP_TO_EXPECT);
        _ConsumeWhiteCharactersUpToNewLine();
        _ConsumeNewLineCharacter();
    }
    if (word == "OUTPUT") {
        _SwitchStateTo(State::READING_LINES_UP_TO_EOF);
        _ConsumeWhiteCharactersUpToNewLine();
        _ConsumeNewLineCharacter();
    }

    if (word == "RUN"
        || word == "WITH"
        || word == "INPUT"
        || word == "EXPECT"
        || word == "OUTPUT") {
        return Token{Token::Kind::KEYWORD, word};
    }
    else if (!word.empty()) {
        return Token{Token::Kind::TEXT, word};
    }
    else {
        return std::nullopt;
    }
}

std::optional<const Token>
Lexer::_HandleReadingLinesUpToExpectState()
{
    const PositionInBuffer beginOfLines = fCurrentPosition;
    const PositionInBuffer beginOfLinesWithNewLine = fCurrentPosition - 1;

    auto endOfLines = fInputBuffer.find("\nEXPECT", beginOfLinesWithNewLine);
    if (endOfLines == std::string::npos) {
        endOfLines = fInputBuffer.size();
    }
    else if (endOfLines == beginOfLinesWithNewLine) {
        endOfLines = beginOfLines;
    }

    _SwitchStateTo(State::READING_KEYWORDS);

    return _ConsumeAndGetTokenWithText(beginOfLines, endOfLines);
}

std::optional<const Token>
Lexer::_HandleReadingLinesUpToEofState()
{
    const PositionInBuffer beginOfLines = fCurrentPosition;
    const PositionInBuffer endOfLines = fInputBuffer.size();

    _SwitchStateTo(State::READING_KEYWORDS);

    return _ConsumeAndGetTokenWithText(beginOfLines, endOfLines);
}

void
Lexer::_SwitchStateTo(const State newState)
{
    fCurrentState = newState;
}

const std::string_view
Lexer::_ReadNextWord()
{
    const PositionInBuffer wordBegin = _FindNextLetterPosition(fCurrentPosition);
    const PositionInBuffer wordEnd = _FindNextWhiteCharPosition(wordBegin);
    const PositionInBuffer wordSize = wordEnd - wordBegin;

    fCurrentPosition = wordEnd;

    return std::string_view(fInputBuffer.data() + wordBegin, wordSize);
}

Lexer::PositionInBuffer
Lexer::_FindNextLetterPosition(const Lexer::PositionInBuffer begin) const
{
    const auto isLetter = [](auto c) {
        return !std::isspace(static_cast<unsigned char>(c));
    };

    return find_next_position(fInputBuffer, begin, isLetter);
}

Lexer::PositionInBuffer
Lexer::_FindNextWhiteCharPosition(const Lexer::PositionInBuffer begin) const
{
    const auto isWhiteChar = [](auto c) {
        return std::isspace(static_cast<unsigned char>(c));
    };

    return find_next_position(fInputBuffer, begin, isWhiteChar);
}

void
Lexer::_ConsumeWhiteCharactersUpToNewLine()
{
    fCurrentPosition = find_next_position(fInputBuffer,
                                          fCurrentPosition,
                                          [](auto c) { return c != ' ' && c != '\t'; });
}

void
Lexer::_ConsumeNewLineCharacter()
{
    if (fCurrentPosition < fInputBuffer.size()) {
        const char character = fInputBuffer.at(fCurrentPosition);

        if (character == '\n') {
            fCurrentPosition++;
        }
        else {
            std::stringstream stream;
            stream << "Unexpected character '" << character << "' at " << fCurrentPosition << " byte.";
            throw exception::UnexpectedCharacterException(stream.str());
        }
    }
}

std::optional<const Token>
Lexer::_ConsumeAndGetTokenWithText(const Lexer::PositionInBuffer begin, const Lexer::PositionInBuffer end)
{
    fCurrentPosition = end;

    return Token{Token::Kind::TEXT,
                 std::string_view(fInputBuffer.data() + begin,
                                  end - begin)};
}

}  // omtt::lexer
