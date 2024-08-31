/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
