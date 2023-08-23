/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
