%{
#include "scanner.hpp"
#include "scene/scene.hpp"

#include <cstring>
#define YY_USER_ACTION yylloc->columns(yyleng);
%}

%option nodefault yyclass="scanner" noyywrap c++
%%
%{
    yylloc->step();
%}

"//"[^\n\r]*(\n|\r|\r\n)            { yylloc->lines(yyleng); yylloc->step(); /* Ignore whitespace. */ }
-{0,1}[0-9]+(\.[0-9]*){0,1}         { yylval->fval = atof(yytext); return Bison::Parser::token::FLOAT; }
"scene"                             { return Bison::Parser::token::SCENE; }
"shape"                             { return Bison::Parser::token::SHAPE; }
\"[a-zA-Z0-9._/-]+\"                { yylval->sval = strdup(yytext); return Bison::Parser::token::FILEPATH; }
"("                                 { return yytext[0]; }
")"                                 { return yytext[0]; }
"{"                                 { return yytext[0]; }
"}"                                 { return yytext[0]; }
","                                 { return yytext[0]; }
.                                   { yylloc->step(); }
"\n"+                               { yylloc->lines(yyleng); yylloc->step(); }
