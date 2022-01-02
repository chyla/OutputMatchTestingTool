/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/lexer/Token.hpp"
#include "headers/parser/exception/detail/Concatenate.hpp"

#include <stdexcept>
#include <string>


namespace omtt::parser::exception
{

class WrongTokenException : public std::runtime_error {
public:
    explicit WrongTokenException(const std::initializer_list<const std::string> expectedValues,
                                 const lexer::TokenKind expectedKind,
                                 const lexer::Token &given)
        :
        std::runtime_error("Expected "
                           + detail::concatenate(expectedValues)
                           + " ("
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
