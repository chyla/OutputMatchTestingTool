/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/lexer/exception/InvalidTokenKindToStringConversionException.hpp"

#include <string>


namespace omtt::lexer
{

enum class TokenKind {
    KEYWORD,
    TEXT,
    INTEGER,
    COMMENT
};

inline std::string
to_string(const TokenKind kind) {
    switch (kind) {
        case TokenKind::KEYWORD:
            return "KEYWORD";
        case TokenKind::TEXT:
            return "TEXT";
        case TokenKind::INTEGER:
            return "INTEGER";
        case TokenKind::COMMENT:
            return "COMMENT";
    };

    throw exception::InvalidTokenKindToStringConversionException(kind);
}

}
