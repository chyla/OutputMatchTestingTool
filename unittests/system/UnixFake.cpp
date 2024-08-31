/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "unittests/system/UnixFake.hpp"


namespace omtt::system::unix
{

const Pipe
MakePipe(const PipeOptions option)
{
    return GlobalFake().MakePipeAction(option);
}

ssize_t
Read(int fd, void *buf, size_t count)
{
    return GlobalFake().ReadAction(fd, buf, count);
}

ssize_t
Write(int fd, const void *buf, size_t count, WriteOptions options)
{
  return GlobalFake().WriteAction(fd, buf, count, options);
}

void
Close(int fd)
{
    GlobalFake().CloseAction(fd);
}

ssize_t
Fork()
{
    return GlobalFake().ForkAction();
}

int
WaitPid(pid_t pid, int *wstatus, int options)
{
    return GlobalFake().WaitPidAction(pid, wstatus, options);
}

void
DuplicateFd(int oldFd, int newFd)
{
    GlobalFake().DuplicateFdAction(oldFd, newFd);
}

void
Exec(const std::string &path, const std::vector<std::string> &arguments)
{
    GlobalFake().ExecAction(path, arguments);
}

void
Terminate(const int status)
{
    GlobalFake().TerminateAction(status);
}

void
SigAction(int signum, const struct sigaction *act, struct sigaction *oldact)
{
    GlobalFake().SigAction(signum, act, oldact);
}

void
Signal(int signum, sighandler_t handler)
{
    GlobalFake().Signal(signum, handler);
}

int
Poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    return GlobalFake().PollAction(fds, nfds, timeout);
}

int
Fcntl(int fd, int cmd, int arg)
{
    return GlobalFake().FcntlAction(fd, cmd, arg);
}

void
Kill(pid_t pid, int sig)
{
    return GlobalFake().KillAction(pid, sig);
}

}  // omtt::system::unix
