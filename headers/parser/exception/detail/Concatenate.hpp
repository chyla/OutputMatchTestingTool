/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
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
