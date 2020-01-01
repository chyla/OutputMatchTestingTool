/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once


namespace omtt::lexer::detail {

enum class State {
    READING_KEYWORDS,
    READING_LINES_UP_TO_EXPECT,
    READING_LINES_UP_TO_EOF
};

}  // omtt::lexer::detail
