/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <stdexcept>


namespace omtt::parser::exception
{

class MissingTextException : public std::runtime_error {
public:
    explicit MissingTextException()
        :
        std::runtime_error("Expected input text, but got nothing.")
    {
    }
};

}  // omtt::parser::exception
