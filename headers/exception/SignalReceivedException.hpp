/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <stdexcept>
#include <string>


namespace omtt::exception
{

class SignalReceivedException : public std::runtime_error
{
public:
    explicit SignalReceivedException(int signum)
        :
      std::runtime_error("System signal received (signum: " + std::to_string(signum) + ").")
    {
    }
};

}
