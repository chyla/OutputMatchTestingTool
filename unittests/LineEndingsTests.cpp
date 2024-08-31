/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
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
