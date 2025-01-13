/*
 * Copyright (c) 2019-2025, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdexcept>


namespace omtt::parser::exception
{

class MissingIntegerException : public std::runtime_error {
public:
    explicit MissingIntegerException()
        :
        std::runtime_error("Expected number, but got nothing.")
    {
    }
};

}  // omtt::parser::exception
