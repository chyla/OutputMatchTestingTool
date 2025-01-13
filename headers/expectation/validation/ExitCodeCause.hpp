/*
 * Copyright (c) 2019-2025, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
