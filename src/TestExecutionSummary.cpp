/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/TestExecutionSummary.hpp"


namespace omtt
{

std::ostream&
operator<<(std::ostream &os, const TestExecutionSummary &tes)
{
    os << "Verdict: " <<  to_cstring(tes.verdict);
    return os;
}

}  // omtt
