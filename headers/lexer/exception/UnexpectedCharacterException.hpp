/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdexcept>


namespace omtt::lexer::exception {

class UnexpectedCharacterException : public std::runtime_error {
public:
    explicit UnexpectedCharacterException(const std::string &msg)
        :
        std::runtime_error(msg)
    {
    }
};

}  // omtt::lexer::exception
