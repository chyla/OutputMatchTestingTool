/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
