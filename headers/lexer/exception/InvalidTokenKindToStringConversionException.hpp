/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/lexer/detail/to_hex_string.hpp"

#include <stdexcept>


namespace omtt::lexer
{

enum class TokenKind;

}

namespace omtt::lexer::exception
{

class InvalidTokenKindToStringConversionException : public std::logic_error {
public:
    explicit InvalidTokenKindToStringConversionException(const omtt::lexer::TokenKind kind)
        :
        std::logic_error("Invalid handling of TokenKind ("
                         + detail::to_hex_string(static_cast<unsigned>(kind))
                         + ") during conversion. Should never happend, please report.")
    {
    }
};

}
