/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
