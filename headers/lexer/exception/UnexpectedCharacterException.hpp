/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
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
