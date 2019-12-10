/*
 * Copyright (c) 2019, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/RunProcess.hpp"
#include "headers/system/Unix.hpp"

#include <array>
#include <iostream>


namespace omtt
{

namespace
{

bool
IsParentProcess(int pid)
{
    return pid > 0;
}

std::string
ReadFromFd(int fd)
{
    std::string output;
    char buf[1024];
    int bytes;

    do {
        bytes = system::unix::Read(fd, buf, sizeof(buf) - 1);
        buf[bytes] = 0;
        output += buf;
    } while (bytes > 0);

    return output;
}

void
WriteToFd(int fd, const std::string_view &buf)
{
    std::string_view::size_type wrote = 0;

    while (wrote < buf.length()) {
        wrote += system::unix::Write(fd, buf.data() + wrote, buf.length() - wrote);
    }
}

void
RedirectPipe(int oldFd, int newFd)
{
    system::unix::Close(newFd);
    system::unix::DuplicateFd(oldFd, newFd);
}

}

ProcessResults
RunProcess(const std::string &path, const std::string_view &input)
{
    ProcessResults results;

    const auto toParentPipe = system::unix::MakePipe();
    const auto toChildPipe = system::unix::MakePipe();

    const auto pid = system::unix::Fork();

    if (IsParentProcess(pid)) {
        system::unix::Close(toParentPipe.writeEnd);
        system::unix::Close(toChildPipe.readEnd);

        WriteToFd(toChildPipe.writeEnd, input);
        system::unix::Close(toChildPipe.writeEnd);

        results.output = ReadFromFd(toParentPipe.readEnd);
        results.exitCode = system::unix::ExitStatus(pid);

        system::unix::Close(toParentPipe.readEnd);
    }
    else {
        system::unix::Close(toParentPipe.readEnd);
        system::unix::Close(toChildPipe.writeEnd);

        RedirectPipe(toChildPipe.readEnd, static_cast<int>(system::unix::FdId::STDIN));
        RedirectPipe(toParentPipe.writeEnd, static_cast<int>(system::unix::FdId::STDOUT));

        system::unix::Exec(path);
    }

    return results;
}

}  // omtt
