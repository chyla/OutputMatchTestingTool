/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "headers/system/Unix.hpp"

#include <functional>


namespace omtt::system::unix
{

struct UnixFake
{
    std::function<const Pipe()>  MakePipeAction;
    std::function<ssize_t (int, void *, size_t)> ReadAction;
    std::function<ssize_t (int, const void *, size_t)> WriteAction;
    std::function<void (int)> CloseAction;
    std::function<ssize_t ()> ForkAction;
    std::function<pid_t (int)> ExitStatusAction;
    std::function<void (int, int)> DuplicateFdAction;
    std::function<void (const std::string &)> ExecAction;
};

inline UnixFake& GlobalFake()
{
    static UnixFake fake;
    return fake;
}

inline void ResetGlobalFake()
{
    auto &fake = GlobalFake();
    fake = system::unix::UnixFake();
}

}  // omtt::system::unix
