/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <string>


namespace omtt::lexer
{

enum class TokenKind {
    KEYWORD,
    TEXT
};

inline std::string
to_string(TokenKind kind) {
    switch (kind) {
        case TokenKind::KEYWORD:
            return "KEYWORD";
        case TokenKind::TEXT:
            return "TEXT";
    };
}

}
