/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
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
const char * const to_cstring(const Verdict verdict)
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
