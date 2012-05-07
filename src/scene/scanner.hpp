#pragma once

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

// Re-declare yylex to be part of this class.
#undef YY_DECL
#define YY_DECL int scanner::yylex()

#include <iostream>
#include "sceneparser.tab.hpp"

class scanner : public yyFlexLexer{
    public:
        scanner(std::istream* in = 0, std::ostream* out = 0) : yyFlexLexer(in, out)
        {}

        inline int yylex(Bison::Parser::semantic_type* lval, Bison::Parser::location_type* lloc){
            yylval = lval;
            yylloc = lloc;
            return yylex();
        }

    private:
        // Make these private to force use of the provided public methods.
        int yylex();
        scanner();
        scanner(const scanner& s);
        scanner& operator=(const scanner& s);

        Bison::Parser::semantic_type* yylval;
        Bison::Parser::location_type* yylloc;
};
