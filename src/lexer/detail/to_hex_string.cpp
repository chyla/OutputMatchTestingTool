/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/lexer/detail/to_hex_string.hpp"


namespace omtt::lexer::detail
{

std::string
to_hex_string(unsigned x)
{
    std::stringstream stream;
    stream << "0x" << std::hex << x;
    return stream.str();
}

}  // omtt::lexer::detail
