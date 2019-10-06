/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/lexer/detail/State.hpp"

#include <stdexcept>
#include <sstream>


namespace omtt::lexer::exception
{

class InvalidStateHandlingException : public std::logic_error {
public:
    explicit InvalidStateHandlingException(const detail::State state)
        :
        std::logic_error("Invalid handling of state "
                         + to_hex_string(static_cast<unsigned>(state))
                         + ". Should never happend, please report.")
    {
    }

private:
    static std::string
    to_hex_string(unsigned x)
    {
        std::stringstream stream;
        stream << "0x" << std::hex << x;
        return stream.str();
    }
};

}
