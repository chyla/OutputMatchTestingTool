/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
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
#include <string>
#include <string_view>


namespace omtt::lexer
{

using detail::State;


namespace
{

const lexer::exception::UnexpectedCharacterException
prepare_unexpected_character_exception(const char character,
                                       const std::string_view::size_type position)
{
    std::stringstream stream;
    stream << "Unexpected character '" << character << "' at " << position << " byte.";
    return exception::UnexpectedCharacterException(stream.str());
}

void
throw_when_word_is_not_a_number(const std::string_view &word,
                                const std::string_view::size_type position)
{
    for (std::string_view::size_type i = 0; i < word.size(); i++) {
        const char character = word.at(i);
        if (!std::isdigit(character)) {
            throw prepare_unexpected_character_exception(character, position + i);
        }
    }
}

template<class UnaryPredicate, class PositionInBuffer>
PositionInBuffer
find_next_position(const std::string &buffer,
                   PositionInBuffer begin,
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
    fCurrentState = State::READ_KEYWORDS_AND_MOVE_TO_READING_LINES;
}

std::optional<const Token>
Lexer::FindNextToken()
{
    switch (fCurrentState) {
        case State::READ_KEYWORDS_AND_MOVE_TO_READING_LINES:
            return _HandleReadingKeywordsState(
                Lexer::ReadingKeywordsStateOptions::MOVE_TO_READING_LINES_STATE_AFTER_INPUT_OUTPUT);

        case State::READ_KEYWORDS:
            return _HandleReadingKeywordsState();

        case State::READ_LINES_UP_TO_EXPECT:
            return _HandleReadingLinesUpToExpectState();

        case State::READ_INTEGER:
            return _HandleReadingInteger();
    };

    throw exception::InvalidStateHandlingException(fCurrentState);
}

std::optional<const Token>
Lexer::_HandleReadingKeywordsState(Lexer::ReadingKeywordsStateOptions options)
{
    const std::string_view word = _ReadNextWord();

    if (options == Lexer::ReadingKeywordsStateOptions::MOVE_TO_READING_LINES_STATE_AFTER_INPUT_OUTPUT
        && (word == "INPUT" || word == "OUTPUT")) {
        _SwitchStateTo(State::READ_LINES_UP_TO_EXPECT);
        _ConsumeWhiteCharactersWithoutNewLine();
        _ConsumeNewLineCharacter();
    }
    if (word == "CODE") {
        _SwitchStateTo(State::READ_INTEGER);
    }
    if (word == "EXPECT") {
        _SwitchStateTo(State::READ_KEYWORDS_AND_MOVE_TO_READING_LINES);
    }

    if (word == "RUN"
        || word == "WITH"
        || word == "INPUT"
        || word == "EXPECT"
        || word == "OUTPUT"
        || word == "EXIT"
        || word == "CODE"
        || word == "IN") {
        return Token{TokenKind::KEYWORD, word};
    }
    else if (word == "EMPTY") {
        _SwitchStateTo(State::READ_KEYWORDS);
        return Token{TokenKind::KEYWORD, word};
    }
    else if (!word.empty()) {
        return Token{TokenKind::TEXT, word};
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

    _SwitchStateTo(State::READ_KEYWORDS_AND_MOVE_TO_READING_LINES);

    return _ConsumeAndGetTokenWithText(beginOfLines, endOfLines);
}

std::optional<const Token>
Lexer::_HandleReadingInteger()
{
    const auto word = _ReadNextWord();

    if (word != "") {
        throw_when_word_is_not_a_number(word, fCurrentPosition - word.size());

        _SwitchStateTo(State::READ_KEYWORDS_AND_MOVE_TO_READING_LINES);

        return Token{TokenKind::INTEGER,
                     word};
    }
    else {
        return std::nullopt;
    }
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
Lexer::_ConsumeWhiteCharactersWithoutNewLine()
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
            throw prepare_unexpected_character_exception(character, fCurrentPosition);
        }
    }
}

std::optional<const Token>
Lexer::_ConsumeAndGetTokenWithText(const Lexer::PositionInBuffer begin, const Lexer::PositionInBuffer end)
{
    fCurrentPosition = end;

    return Token{TokenKind::TEXT,
                 std::string_view(fInputBuffer.data() + begin,
                                  end - begin)};
}

}  // omtt::lexer
