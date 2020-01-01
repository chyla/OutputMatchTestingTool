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

class WrongTokenException : public std::runtime_error {
public:
    explicit WrongTokenException(const std::string &expectedValue,
                                 const lexer::TokenKind expectedKind,
                                 const lexer::Token &given)
        :
        std::runtime_error("Expected '"
                           + expectedValue
                           + "' ("
                           + to_string(expectedKind)
                           + "), but got '"
                           + static_cast<std::string>(given.value)
                           + "' ("
                           + to_string(given.kind)
                           + ").")
    {
    }
};

}  // omtt::parser::exception
