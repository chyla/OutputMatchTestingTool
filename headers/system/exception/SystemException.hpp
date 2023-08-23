/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
