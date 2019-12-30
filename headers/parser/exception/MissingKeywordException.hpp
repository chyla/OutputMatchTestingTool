/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <stdexcept>
#include <string>


namespace omtt::parser::exception
{

class MissingKeywordException : public std::runtime_error {
public:
  explicit MissingKeywordException(const std::string &expectedKeyword)
        :
        std::runtime_error("Expected '" + expectedKeyword + "' (KEYWORD), but got nothing.")
    {
    }
};

}  // omtt::parser::exception