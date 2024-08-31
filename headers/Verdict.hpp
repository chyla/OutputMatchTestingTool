/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdexcept>


namespace omtt
{

enum class Verdict
{
    PASS,
    FAIL
};


inline
const char *to_cstring(const Verdict verdict)
{
    if (verdict == Verdict::PASS) {
        return "PASS";
    }
    else if (verdict == Verdict::FAIL) {
        return "FAIL";
    }
    else {
        throw std::logic_error("Unexpected Verdict value to string conversion.");
    }
}

}  // omtt
