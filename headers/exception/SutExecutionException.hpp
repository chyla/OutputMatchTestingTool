/*
 * Copyright (c) 2019-2021, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <stdexcept>


namespace omtt::exception
{

class SutExecutionException : public std::runtime_error
{
public:
    explicit SutExecutionException(const std::string &msg)
        :
        std::runtime_error(msg)
    {
    }
};

}
