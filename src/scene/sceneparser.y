%skeleton "lalr1.cc"
%defines
%locations
%define namespace "Bison"
%define parser_class_name "Parser"

%parse-param    {scanner& scan}
%parse-param    {scene& scn}
%lex-param      {scanner& scan}

%code requires {
    #include <string>
    #include "scene/scene.hpp"

    // We want to return a string
    //#define YYSTYPE std::string

    class scanner;
}

%code{
    // Prototype for the yylex function
    static int yylex(Bison::Parser::semantic_type * yylval,
                     Bison::Parser::location_type * yylloc,
                     scanner &scan);
}

%union{
    float fval;
    char* sval;
}

%token SCENE SHAPE FILEPATH

%token <fval> FLOAT

%%

scenefile :
          SCENE '{' contents '}'
          ;

contents :
        shape contents |
        shape
        ;

shape :
       SHAPE '{' '}'
       { std::cerr << "Made shape!" << endl; }
       ;
%%
void Bison::Parser::error(const Bison::Parser::location_type& loc, const std::string& msg){
    cerr << "Parse error at: " << loc << ": " << msg<< endl;
}

#include "scanner.hpp"
static int yylex(Bison::Parser::semantic_type * yylval,
                 Bison::Parser::location_type * yylloc,
                 scanner &scan) {
    return scan.yylex(yylval, yylloc);
}
