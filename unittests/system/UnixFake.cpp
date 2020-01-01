/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "UnixFake.hpp"


namespace omtt::system::unix
{

const Pipe
MakePipe()
{
    return GlobalFake().MakePipeAction();
}

ssize_t
Read(int fd, void *buf, size_t count)
{
    return GlobalFake().ReadAction(fd, buf, count);
}

ssize_t
Write(int fd, const void *buf, size_t count)
{
    return GlobalFake().WriteAction(fd, buf, count);
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

pid_t
ExitStatus(int pid)
{
    return GlobalFake().ExitStatusAction(pid);
}

void
DuplicateFd(int oldFd, int newFd)
{
    GlobalFake().DuplicateFdAction(oldFd, newFd);
}

void
Exec(const std::string &path)
{
    GlobalFake().ExecAction(path);
}

}  // omtt::system::unix
