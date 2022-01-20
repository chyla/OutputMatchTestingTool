/*
 * Copyright (c) 2019-2022, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
 */

#include "unittests/test_framework.hpp"

#include "headers/LineEndings.hpp"


namespace omtt
{

TEST_CASE("Single LF should not be changed")
{
    std::string text = "\n";
    const std::string expectedOutputText = "\n";

    changeLineEndingsToLf(text);

    CHECK(text == expectedOutputText);
}

TEST_CASE("Multiple LF should not be changed")
{
    std::string text = "\n\n\n";
    const std::string expectedOutputText = "\n\n\n";

    changeLineEndingsToLf(text);

    CHECK(text == expectedOutputText);
}

TEST_CASE("Single CR should be changed to LF")
{
    std::string text = "\r";
    const std::string expectedOutputText = "\n";

    changeLineEndingsToLf(text);

    CHECK(text == expectedOutputText);
}

TEST_CASE("Multiple CR should be changed to LF")
{
    std::string text = "\r\r\r";
    const std::string expectedOutputText = "\n\n\n";

    changeLineEndingsToLf(text);

    CHECK(text == expectedOutputText);
}

TEST_CASE("Single CR LF should be changed to LF")
{
    std::string text = "\r\n";
    const std::string expectedOutputText = "\n";

    changeLineEndingsToLf(text);

    CHECK(text == expectedOutputText);
}

TEST_CASE("Multiple CR LF should be changed to LF")
{
    std::string text = "\r\n\r\n\r\n";
    const std::string expectedOutputText = "\n\n\n";

    changeLineEndingsToLf(text);

    CHECK(text == expectedOutputText);
}

TEST_CASE("Single CR LF and CR should be changed to LF LF")
{
    std::string text = "\r\n\r";
    const std::string expectedOutputText = "\n\n";

    changeLineEndingsToLf(text);

    CHECK(text == expectedOutputText);
}

TEST_CASE("Text other than CR, CR LF should not be modified")
{
    std::string text = "Hello\r\nWorld\r";
    const std::string expectedOutputText = "Hello\nWorld\n";

    changeLineEndingsToLf(text);

    CHECK(text == expectedOutputText);
}

}
