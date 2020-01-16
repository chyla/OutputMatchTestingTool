/*
 * Copyright (c) 2019-2020, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "headers/expectation/FullOutputExpectation.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iterator>
#include <sstream>


namespace omtt::expectation
{

namespace
{

constexpr int CONTEXT_SIZE = 6;

std::string::size_type
find_first_difference_position(const std::string_view &expectedOutput, const std::string &output)
{
    auto diff = std::mismatch(output.begin(), output.end(), expectedOutput.begin());
    return std::distance(output.begin(), diff.first);
}

template<class T>
void
write_char(std::ostream &os, const T c)
{
    os << std::setw(5) << std::left << c;
}

void
write_hex_integer(std::ostream &os, const int c)
{
    os << "0x" << std::setfill('0') << std::setw(2) << std::hex << c << " ";
}

template<class T>
T
start_position(const T mismatchPosition)
{
    T lower = 0;
    if (mismatchPosition > CONTEXT_SIZE) {
        lower = mismatchPosition - CONTEXT_SIZE;
    }
    return lower;
}

template<class T>
T
end_position(const T textLength, const T mismatchPosition)
{
    return std::min(textLength,
                    mismatchPosition + CONTEXT_SIZE + 1);
}

bool
isSpace(const int c) {
    return c == ' ';
}

bool
isTab(const int c) {
    return c == '\t';
}

bool
isCr(const int c) {
    return c == '\r';
}

bool
isLf(const int c) {
    return c == '\n';
}

bool
isVTab(const int c) {
    return c == '\v';
}

bool
isFeed(const int c) {
    return c == '\f';
}

template<class T>
std::string
context(const T &text, const typename T::size_type mismatchPosition)
{
    std::stringstream alnum, point, hex;

    const typename T::size_type lower = start_position(mismatchPosition);
    const typename T::size_type upper = end_position(text.length(), mismatchPosition);
    for (auto i = lower; i < upper; ++i) {
        const char c = text.at(i);
        if (isSpace(c)) {
            write_char(alnum, "SPC");
        }
        else if (isTab(c)) {
            write_char(alnum, "TAB");
        }
        else if (isCr(c)) {
            write_char(alnum, "CR");
        }
        else if (isLf(c)) {
            write_char(alnum, "LF");
        }
        else if (isVTab(c)) {
            write_char(alnum, "VT");
        }
        else if (isFeed(c)) {
            write_char(alnum, "FF");
        }
        else if (std::isprint(c)) {
            write_char(alnum, c);
        }
        else {
            write_char(alnum, "NP");
        }
        write_char(point, (i == mismatchPosition) ? '^' : ' ');
        write_hex_integer(hex, c);
    }

    return alnum.str() + "\n" + point.str() + "\n" + hex.str();
}

}


ValidationResult
FullOutputExpectation::Validate(const ProcessResults &processResults)
{
    ValidationResult result;

    if (fExpectedOutput == processResults.output) {
        result.isSatisfied = true;
        result.cause = std::nullopt;
    }
    else {
        const auto differencePosition = find_first_difference_position(fExpectedOutput, processResults.output);
        result.isSatisfied = false;
        result.cause =
            "Output doesn't match.\n"
            "First difference at byte: " + std::to_string(differencePosition) + "\n"
          + "Expected context:\n" + context(fExpectedOutput, differencePosition) + "\n"
          + "Output context:\n" + context(processResults.output, differencePosition);
    }

    return result;
}

}  // omtt::expectation
