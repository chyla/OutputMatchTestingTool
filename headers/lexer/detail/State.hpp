/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once


namespace omtt::lexer::detail
{

enum class State {
    READ_KEYWORDS_AND_MOVE_TO_READING_LINES,
    READ_KEYWORDS,
    READ_LINES_UP_TO_EXPECT,
    READ_INTEGER,
    READ_COMMENT
};

}  // omtt::lexer::detail
