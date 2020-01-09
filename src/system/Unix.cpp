/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/system/Unix.hpp"
#include "headers/system/exception/SystemException.hpp"

#include <cerrno>
#include <limits>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


namespace omtt::system::unix
{

const Pipe
MakePipe()
{
    int fd[2];
    int err = pipe(fd);
    if (err < 0) {
        throw exception::SystemException("failure in pipe()", errno);
    }

    return {fd[0], fd[1]};
}

ssize_t
Read(int fd, void *buf, size_t count)
{
    ssize_t bytes = read(fd, buf, count);
    if (bytes < 0) {
        throw exception::SystemException("failure in read()", errno);
    }

    return bytes;
}

ssize_t
Write(int fd, const void *buf, size_t count)
{
    ssize_t bytes = write(fd, buf, count);
    if (bytes < 0) {
        throw exception::SystemException("failure in write()", errno);
    }

    return bytes;
}

void
Close(int fd)
{
    int err = close(fd);
    if (err) {
        throw exception::SystemException("failure in close()", errno);
    }
}

ssize_t
Fork()
{
    int pid = fork();
    if (pid < 0) {
        throw exception::SystemException("failure in fork()", errno);
    }

    return pid;
}

int
ExitStatus(int pid)
{
    int status = 0;
    int err = waitpid(pid, &status, 0);
    if (err != pid) {
        throw exception::SystemException("failure in waitpid()", errno);
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    else {
        return std::numeric_limits<int>::max();
    }
}

void
DuplicateFd(int oldFd, int newFd)
{
    int err = dup2(oldFd, newFd);
    if (err < 0) {
        throw exception::SystemException("failure in dup2()", errno);
    }
}

void
Exec(const std::string &path)
{
    int err = execl(path.c_str(), path.c_str(), static_cast<const char*>(nullptr));
    if (err < 0) {
        throw exception::SystemException("failure in execl()", errno);
    }
}

} // omtt::system::unix
