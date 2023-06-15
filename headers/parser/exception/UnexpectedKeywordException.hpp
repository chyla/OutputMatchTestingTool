/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <stdexcept>
#include <string>


namespace omtt::parser::exception
{

class UnexpectedKeywordException : public std::runtime_error {
public:
    explicit UnexpectedKeywordException(const std::string &keyword)
        :
        std::runtime_error("Unexpected keyword '"
                           + keyword
                           + "'.")
    {
    }
};

}  // omtt::parser::exception
