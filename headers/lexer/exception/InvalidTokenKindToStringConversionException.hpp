/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
