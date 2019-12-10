/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#pragma once

#include <string>

#include <sys/types.h>


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

const Pipe
MakePipe();

ssize_t
Read(int fd, void *buf, size_t count);

ssize_t
Write(int fd, const void *buf, size_t count);

void
Close(int fd);

ssize_t
Fork();

pid_t
ExitStatus(int pid);

void
DuplicateFd(int oldFd, int newFd);

void
Exec(const std::string &path);

}  // omtt::system::unix
