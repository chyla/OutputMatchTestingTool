/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "headers/system/Unix.hpp"

#include <functional>


namespace omtt::system::unix
{

struct UnixFake
{
    std::function<const Pipe(const PipeOptions option)>  MakePipeAction;
    std::function<ssize_t (int, void *, size_t)> ReadAction;
    std::function<ssize_t (int, const void *, size_t, WriteOptions)> WriteAction;
    std::function<void (int)> CloseAction;
    std::function<ssize_t ()> ForkAction;
    std::function<int (pid_t pid, int *wstatus, int options)> WaitPidAction;
    std::function<void (int, int)> DuplicateFdAction;
    std::function<void (const std::string &, const std::vector<std::string> &)> ExecAction;
    std::function<void (int)> TerminateAction;
    std::function<void (int, const struct sigaction*, struct sigaction*)> SigAction;
    std::function<void (int, sighandler_t)> Signal;
    std::function<int (struct pollfd *fds, nfds_t nfds, int timeout)> PollAction;
    std::function<int (int fd, int cmd, int arg)> FcntlAction;
    std::function<void (pid_t pid, int sig)> KillAction;
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
