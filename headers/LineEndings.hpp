/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
