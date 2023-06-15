/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/ReadFile.hpp"
#include "headers/LineEndings.hpp"
#include "headers/exception/FileReadException.hpp"

#include <fstream>


namespace omtt
{

constexpr unsigned
operator "" _mb(const long double x)
{
    const auto kb = x * 1024;
    const auto bytes = kb * 1024;
    return bytes;
}

size_t
readFileSize(std::ifstream &stream)
{
    stream.seekg(0, stream.end);
    const size_t fileSize = stream.tellg();
    stream.seekg(0, stream.beg);

    return fileSize;
}


std::string
readFile(const std::string &path)
{
    constexpr size_t tmpSize = 0.5_mb;
    char tmp[tmpSize];
    std::string outputBuffer;
    std::ifstream file(path.c_str());

    if (!file.good()) {
        throw exception::FileReadException("failed to open file: " + path);
    }

    outputBuffer.reserve(readFileSize(file));

    while (!file.eof()) {
        file.read(tmp, tmpSize);
        outputBuffer.append(tmp, file.gcount());
    }

    file.close();

    changeLineEndingsToLf(outputBuffer);

    return outputBuffer;
}

}  // omtt
