/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once


namespace omtt::lexer::detail
{

enum class State {
    READ_KEYWORDS_AND_MOVE_TO_READING_LINES,
    READ_KEYWORDS,
    READ_LINES_UP_TO_EXPECT,
    READ_INTEGER
};

}  // omtt::lexer::detail
