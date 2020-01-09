/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/parser/exception/detail/Concatenate.hpp"

#include <stdexcept>
#include <string>


namespace omtt::parser::exception
{

class MissingKeywordException : public std::runtime_error {
public:
    explicit MissingKeywordException(const std::initializer_list<const std::string> &expectedKeywords)
        :
        std::runtime_error("Expected " + detail::concatenate(expectedKeywords) + " (KEYWORD), but got nothing.")
    {
    }
};

}  // omtt::parser::exception
