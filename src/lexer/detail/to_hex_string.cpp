/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
