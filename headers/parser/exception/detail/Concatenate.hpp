/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once


namespace omtt::parser::exception::detail
{

template<class T>
std::string
concatenate(const T &values)
{
    const char * const separator = " or ";
    std::string s;
    bool isFirst = true;
    for (const auto &item : values) {
        if (!isFirst) {
            s += separator;
        }
        s += "'" + item + "'";
        isFirst = false;
    }
    return s;
}

}  // omtt::parser::exception::detail
