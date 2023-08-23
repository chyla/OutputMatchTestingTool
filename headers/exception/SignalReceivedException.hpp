/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
