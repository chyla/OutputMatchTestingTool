/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "headers/system/Unix.hpp"
#include "headers/system/exception/SystemException.hpp"

#include <cerrno>
#include <limits>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


namespace omtt::system::unix
{

const Pipe
MakePipe(const PipeOptions option)
{
    int fd[2];
    int pipeFlags = 0;
    if (option == PipeOptions::CLOSE_ON_EXEC) {
        pipeFlags |= O_CLOEXEC;
    }

    const int err = pipe2(fd, pipeFlags);
    if (err < 0) {
        throw exception::SystemException("failure in pipe()", errno);
    }

    return {fd[0], fd[1]};
}

ssize_t
Read(int fd, void *buf, size_t count)
{
    const ssize_t bytes = read(fd, buf, count);
    if (bytes < 0) {
        throw exception::SystemException("failure in read()", errno);
    }

    return bytes;
}

ssize_t
Write(int fd, const void *buf, size_t count, WriteOptions options)
{
    const ssize_t bytes = write(fd, buf, count);
    if (bytes < 0) {
        if (options == system::unix::WriteOptions::IGNORE_EPIPE_EAGAIN
                && (errno == EPIPE || errno == EAGAIN)) {
            return 0;
        }
        else {
            throw exception::SystemException("failure in write()", errno);
        }
    }

    return bytes;
}

void
Close(int fd)
{
    const int err = close(fd);
    if (err) {
        throw exception::SystemException("failure in close()", errno);
    }
}

ssize_t
Fork()
{
    const int pid = fork();
    if (pid < 0) {
        throw exception::SystemException("failure in fork()", errno);
    }

    return pid;
}

int
WaitPid(pid_t pid, int *wstatus, int options)
{
    const int ret = waitpid(pid, wstatus, options);
    if (ret < 0) {
        throw exception::SystemException("failure in waitpid()", errno);
    }
    return ret;
}

void
DuplicateFd(int oldFd, int newFd)
{
    const int err = dup2(oldFd, newFd);
    if (err < 0) {
        throw exception::SystemException("failure in dup2()", errno);
    }
}

void
Exec(const std::string &path, const std::vector<std::string> &arguments)
{
    constexpr std::vector<std::string>::size_type ARGUMENTS_MAX_SIZE = 250;
    if (arguments.size() > ARGUMENTS_MAX_SIZE) {
        throw exception::SystemException("too much arguments to exec", errno);
    }

    char *args[ARGUMENTS_MAX_SIZE + 2] = {0};
    args[0] = const_cast<char*>(path.c_str());
    for (std::vector<std::string>::size_type i = 0; i < arguments.size(); ++i) {
        args[i + 1] = const_cast<char*>(arguments.at(i).c_str());
    }

    const int err = execv(path.c_str(), static_cast<char * const *>(args));
    if (err < 0) {
        throw exception::SystemException("failure in execv()", errno);
    }
}

void
Terminate(const int status)
{
    _exit(status);
}

void
SigAction(int signum, const struct sigaction *act, struct sigaction *oldact)
{
    const int err = sigaction(signum, act, oldact);
    if (err < 0) {
        throw exception::SystemException("failure in sigaction()", errno);
    }
}

void
Signal(int signum, signal_handler handler)
{
    const auto oldHandler = signal(signum, handler);
    if (oldHandler == SIG_ERR) {
        throw exception::SystemException("failure in signal()", errno);
    }
}

int
Poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    const int count = poll(fds, nfds, timeout);
    if (count < 0) {
        throw exception::SystemException("failure in poll()", errno);
    }
    return count;
}

int
Fcntl(int fd, int cmd, int arg)
{
    const int ret = fcntl(fd, cmd, arg);
    if (ret < 0) {
        throw exception::SystemException("failure in fcntl()", errno);
    }
    return ret;
}

void
Kill(pid_t pid, int sig)
{
    const int ret = kill(pid, sig);
    if (ret < 0) {
        throw exception::SystemException("failure in kill()", errno);
    }
}


} // omtt::system::unix
