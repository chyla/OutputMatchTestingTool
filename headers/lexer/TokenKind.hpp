/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
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
    TEXT
};

inline std::string
to_string(const TokenKind kind) {
    switch (kind) {
        case TokenKind::KEYWORD:
            return "KEYWORD";
        case TokenKind::TEXT:
            return "TEXT";
    };

    throw exception::InvalidTokenKindToStringConversionException(kind);
}

}
