/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/lexer/TokenKind.hpp"

#include <string_view>


namespace omtt::lexer
{

struct Token {
    const TokenKind kind;
    const std::string_view value;
};

}  // omtt::lexer
