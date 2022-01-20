/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <string>


namespace omtt
{

namespace
{

void
replace(std::string &buffer, const std::string &what, const std::string &text)
{
    std::string::size_type pos = 0;
    while ((pos = buffer.find(what, pos)) != std::string::npos) {
         buffer.replace(pos, what.length(), text);
         pos += text.length();
    }
}

}

inline void
changeLineEndingsToLf(std::string &buffer)
{
    replace(buffer, "\r\n", "\n");
    replace(buffer, "\r", "\n");
}

}  // omtt
