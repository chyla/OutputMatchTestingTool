/*
 * Copyright (c) 2019-2024, Adam Chyła <adam@chyla.org>.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "headers/lexer/Token.hpp"

#include <initializer_list>
#include <optional>
#include <vector>


namespace omtt::lexer
{

class LexerFake
{
public:
    explicit LexerFake(std::initializer_list<Token> tokens)
        :
        fTokens(tokens)
    {
        fPosition = fTokens.begin();
    }


    std::optional<const Token>
    FindNextToken()
    {
        if (fPosition != fTokens.end()) {
            Token token = *fPosition;
            ++fPosition;
            return token;
        }
        else {
            return std::nullopt;
        }
    }


private:
    std::vector<Token>           fTokens;
    std::vector<Token>::iterator fPosition;
};

}  // omtt::lexer
