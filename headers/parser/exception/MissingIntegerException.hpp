/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
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
