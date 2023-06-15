/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/lexer/detail/to_hex_string.hpp"
#include "headers/lexer/detail/State.hpp"

#include <stdexcept>


namespace omtt::lexer::exception
{

class InvalidStateHandlingException : public std::logic_error {
public:
    explicit InvalidStateHandlingException(const detail::State state)
        :
        std::logic_error("Invalid handling of state "
                         + detail::to_hex_string(static_cast<unsigned>(state))
                         + ". Should never happend, please report.")
    {
    }
};

}
