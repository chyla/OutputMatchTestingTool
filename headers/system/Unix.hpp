/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include "config.h"

#include <string>
#include <vector>

#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


namespace omtt::system::unix
{

enum class FdId
{
    STDIN = 0,
    STDOUT = 1,
    STDERR = 2
};

struct Pipe
{
    const int readEnd;
    const int writeEnd;
};

enum class PipeOptions
{
    NONE,
    CLOSE_ON_EXEC
};

enum class WriteOptions
{
    NONE,
    IGNORE_EPIPE
};

const Pipe
MakePipe(const PipeOptions option = PipeOptions::NONE);

ssize_t
Read(int fd, void *buf, size_t count);

ssize_t
Write(int fd, const void *buf, size_t count, WriteOptions options = WriteOptions::NONE);

void
Close(int fd);

ssize_t
Fork();

int
WaitPid(pid_t pid, int *wstatus, int options);

void
DuplicateFd(int oldFd, int newFd);

void
Exec(const std::string &path, const std::vector<std::string> &arguments);

void
Terminate(const int status);

void
SigAction(int signum, const struct sigaction *act, struct sigaction *oldact);

#ifdef HAVE_SIGHANDLER_T
using signal_handler = sighandler_t;
#else
using signal_handler = sig_t;
#endif

void
Signal(int signum, signal_handler handler);

int
Poll(struct pollfd *fds, nfds_t nfds, int timeout);

int
Fcntl(int fd, int cmd, int arg);

void
Kill(pid_t pid, int sig);

}  // omtt::system::unix
