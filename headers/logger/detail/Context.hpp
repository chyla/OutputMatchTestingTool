/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iterator>
#include <sstream>


namespace omtt::logger::detail
{

namespace
{

constexpr int CONTEXT_SIZE = 6;

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

}


enum class PointerVisibility
{
    NO_POINTER,
    INCLUDE_POINTER
};


template<class T>
std::string
context(const T &text,
        const typename T::size_type mismatchPosition,
        const PointerVisibility pointerVisibility)
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

        if  (pointerVisibility == PointerVisibility::INCLUDE_POINTER) {
            write_char(point, (i == mismatchPosition) ? '^' : ' ');
        }

        write_hex_integer(hex, c);
    }

    if (pointerVisibility == PointerVisibility::INCLUDE_POINTER) {
        return alnum.str() + "\n" + point.str() + "\n" + hex.str();
    }
    else {
        return alnum.str() + "\n" + hex.str();
    }
}

}
