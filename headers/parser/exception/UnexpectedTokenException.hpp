/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/lexer/Token.hpp"

#include <stdexcept>
#include <string>


namespace omtt::parser::exception
{

class UnexpectedTokenException : public std::runtime_error {
public:
    explicit UnexpectedTokenException(const lexer::Token &token)
        :
        std::runtime_error("Unexpected token '"
                           + static_cast<std::string>(token.value)
                           + "' ("
                           + to_string(token.kind)
                           + ").")
    {
    }
};

}  // omtt::parser::exception
