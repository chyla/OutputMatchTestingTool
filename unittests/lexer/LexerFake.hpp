/*
 * Copyright (c) 2019-2023, Adam Chyła <adam@chyla.org>.
 * All rights reserved.
 *
 * Distributed under the terms of the BSD 3-Clause License.
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
