/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <string_view>


namespace omtt::lexer
{

struct Token {
    enum class Kind {
        KEYWORD,
        TEXT
    };

    const Kind kind;
    const std::string_view value;
};

}  // omtt::lexer
