/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
