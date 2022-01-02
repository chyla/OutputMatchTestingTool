/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once


namespace omtt::expectation::validation
{

struct ExitCodeCause
{
    const int fExpectedExitCode;
    const int fExitCode;
};

}
