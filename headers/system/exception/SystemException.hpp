/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <system_error>


namespace omtt::system::unix::exception
{

class SystemException : public std::system_error
{
public:
    SystemException(const char * const msg, const int ec)
        :
        std::system_error(ec, std::system_category(), msg)
    {
    }
};

} // omtt::system::unix::exception
