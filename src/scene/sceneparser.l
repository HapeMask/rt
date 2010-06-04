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

"//"[^\n\r]*(\n|\r|\r\n)                { yylloc->lines(1); yylloc->step(); /* Ignore whitespace. */ }
"-"{0,1}[0-9]+(\.[0-9]*){0,1}|0         { yylval->fval = atof(yytext); return Bison::Parser::token::FLOAT; }

"scene"                                 { return Bison::Parser::token::SCENE; }
"shape"                                 { return Bison::Parser::token::SHAPE; }
"camera"                                { return Bison::Parser::token::CAMERA; }
"light"                                 { return Bison::Parser::token::LIGHT; }

"area"                                  { return Bison::Parser::token::AREATYPE; }
"point"                                 { return Bison::Parser::token::POINTTYPE; }

"triangle"                              { return Bison::Parser::token::TRIANGLE; }
"sphere"                                { return Bison::Parser::token::SPHERE; }
"objfile"                               { return Bison::Parser::token::OBJFILE; }
\"[a-zA-Z0-9._/-]+\"                    { yylval->sval = strdup(yytext); return Bison::Parser::token::FILEPATH; }

"material"                              { return Bison::Parser::token::MATERIAL; }
"emissive"                              { return Bison::Parser::token::EMISSIVE; }
"blinn"                                 { return Bison::Parser::token::BLINN; }
"aniso"                                 { return Bison::Parser::token::ANISO; }
"phong"                                 { return Bison::Parser::token::PHONG; }
"specular"                              { return Bison::Parser::token::SPECULAR; }
"lambert"                               { return Bison::Parser::token::LAMBERT; }
"substrate"                             { return Bison::Parser::token::SUBSTRATE; }
"pair"                                  { return Bison::Parser::token::PAIR; }

"dielectric"                            { return Bison::Parser::token::DIELECTRIC; }
"conductor"                             { return Bison::Parser::token::CONDUCTOR; }

"bvh"                                   { return Bison::Parser::token::BVH; }
"octree"                                { return Bison::Parser::token::OCTREE; }
"default"                               { return Bison::Parser::token::DEFAULT; }

"("                                     { return yytext[0]; }
")"                                     { return yytext[0]; }
"{"                                     { return yytext[0]; }
"}"                                     { return yytext[0]; }
"<"                                     { return yytext[0]; }
">"                                     { return yytext[0]; }
","                                     { return yytext[0]; }
.                                       { yylloc->step(); }
"\n"+                                   { yylloc->lines(yyleng); yylloc->step(); }