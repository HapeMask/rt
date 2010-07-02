/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2010 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* First part of user declarations.  */


/* Line 310 of lalr1.cc  */
#line 39 "sceneparser.tab.c"


#include "sceneparser.tab.h"

/* User implementation prologue.  */


/* Line 316 of lalr1.cc  */
#line 48 "sceneparser.tab.c"
/* Unqualified %code blocks.  */

/* Line 317 of lalr1.cc  */
#line 41 "../src/scene/sceneparser.y"

    // Prototype for the yylex function
    static int yylex(Bison::Parser::semantic_type * yylval,
                     Bison::Parser::location_type * yylloc,
                     scanner &scan);



/* Line 317 of lalr1.cc  */
#line 62 "sceneparser.tab.c"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


/* Line 379 of lalr1.cc  */
#line 4 "../src/scene/sceneparser.y"
namespace Bison {

/* Line 379 of lalr1.cc  */
#line 130 "sceneparser.tab.c"
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  Parser::Parser (scanner& scan_yyarg, scene& scn_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scan (scan_yyarg),
      scn (scn_yyarg)
  {
  }

  Parser::~Parser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  Parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  Parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  Parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  Parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  Parser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc, scan);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 2:

/* Line 677 of lalr1.cc  */
#line 107 "../src/scene/sceneparser.y"
    {
                scn.setAccelerator(acceleratorPtr((yysemantic_stack_[(9) - (3)].aval)));
                scn.setTracer(rayTracerPtr((yysemantic_stack_[(9) - (5)].tval)));
            }
    break;

  case 3:

/* Line 677 of lalr1.cc  */
#line 114 "../src/scene/sceneparser.y"
    { (yyval.aval) = new bvh(); }
    break;

  case 4:

/* Line 677 of lalr1.cc  */
#line 115 "../src/scene/sceneparser.y"
    { (yyval.aval) = new octree(); }
    break;

  case 5:

/* Line 677 of lalr1.cc  */
#line 116 "../src/scene/sceneparser.y"
    { (yyval.aval) = new defaultAccelerator(); }
    break;

  case 6:

/* Line 677 of lalr1.cc  */
#line 120 "../src/scene/sceneparser.y"
    { (yyval.tval) = new whittedRayTracer(scn); }
    break;

  case 7:

/* Line 677 of lalr1.cc  */
#line 121 "../src/scene/sceneparser.y"
    { (yyval.tval) = new pathTracer(scn); }
    break;

  case 8:

/* Line 677 of lalr1.cc  */
#line 122 "../src/scene/sceneparser.y"
    { (yyval.tval) = new bdpt(scn); }
    break;

  case 15:

/* Line 677 of lalr1.cc  */
#line 134 "../src/scene/sceneparser.y"
    {
           cameraPtr cam(new camera((float)(yysemantic_stack_[(27) - (3)].fval), (float)(yysemantic_stack_[(27) - (5)].fval), 0.1f, 100.f, (yysemantic_stack_[(27) - (8)].fval), point3((yysemantic_stack_[(27) - (10)].fval), (yysemantic_stack_[(27) - (12)].fval), (yysemantic_stack_[(27) - (14)].fval)), point3((yysemantic_stack_[(27) - (16)].fval), (yysemantic_stack_[(27) - (18)].fval), (yysemantic_stack_[(27) - (20)].fval)), vec3((yysemantic_stack_[(27) - (22)].fval), (yysemantic_stack_[(27) - (24)].fval), (yysemantic_stack_[(27) - (26)].fval))));
           scn.setCamera(cam);
       }
    break;

  case 16:

/* Line 677 of lalr1.cc  */
#line 141 "../src/scene/sceneparser.y"
    { scn.addLight((yysemantic_stack_[(1) - (1)].lval)); }
    break;

  case 17:

/* Line 677 of lalr1.cc  */
#line 142 "../src/scene/sceneparser.y"
    { scn.addLight((yysemantic_stack_[(1) - (1)].lval)); }
    break;

  case 18:

/* Line 677 of lalr1.cc  */
#line 147 "../src/scene/sceneparser.y"
    { light* l = new pointLight(point3((yysemantic_stack_[(19) - (6)].fval), (yysemantic_stack_[(19) - (8)].fval), (yysemantic_stack_[(19) - (10)].fval)), (yysemantic_stack_[(19) - (18)].fval), rgbColor((yysemantic_stack_[(19) - (12)].fval), (yysemantic_stack_[(19) - (14)].fval), (yysemantic_stack_[(19) - (16)].fval))); (yyval.lval) = l; }
    break;

  case 19:

/* Line 677 of lalr1.cc  */
#line 152 "../src/scene/sceneparser.y"
    { light* l = new areaLight(point3((yysemantic_stack_[(31) - (6)].fval), (yysemantic_stack_[(31) - (8)].fval), (yysemantic_stack_[(31) - (10)].fval)), (yysemantic_stack_[(31) - (30)].fval), rgbColor((yysemantic_stack_[(31) - (24)].fval), (yysemantic_stack_[(31) - (26)].fval), (yysemantic_stack_[(31) - (28)].fval)), vec3((yysemantic_stack_[(31) - (12)].fval), (yysemantic_stack_[(31) - (14)].fval), (yysemantic_stack_[(31) - (16)].fval)), vec3((yysemantic_stack_[(31) - (18)].fval), (yysemantic_stack_[(31) - (20)].fval), (yysemantic_stack_[(31) - (22)].fval))); (yyval.lval) = l; }
    break;

  case 20:

/* Line 677 of lalr1.cc  */
#line 156 "../src/scene/sceneparser.y"
    {
           shapePtr shp(new shape());
           arraylist<primitive*>::iterator it;
           for(it = (yysemantic_stack_[(5) - (3)].listval)->begin(); it != (yysemantic_stack_[(5) - (3)].listval)->end(); ++it){
               shp->addPrimitive(*it);
           }
           shp->setMaterial((yysemantic_stack_[(5) - (4)].mval));
           scn.addShape(shp);
       }
    break;

  case 21:

/* Line 677 of lalr1.cc  */
#line 165 "../src/scene/sceneparser.y"
    { (yysemantic_stack_[(5) - (3)].shval)->setMaterial((yysemantic_stack_[(5) - (4)].mval)); scn.addShape((yysemantic_stack_[(5) - (3)].shval)); }
    break;

  case 22:

/* Line 677 of lalr1.cc  */
#line 170 "../src/scene/sceneparser.y"
    { shape* s = new triangleMesh(); objParser::parse(std::string((yysemantic_stack_[(7) - (6)].sval)).substr(1, std::string((yysemantic_stack_[(7) - (6)].sval)).length() - 2), s); (yyval.shval) = s; }
    break;

  case 23:

/* Line 677 of lalr1.cc  */
#line 173 "../src/scene/sceneparser.y"
    { shape* s = new shape(); objParser::parse(std::string((yysemantic_stack_[(7) - (6)].sval)).substr(1, std::string((yysemantic_stack_[(7) - (6)].sval)).length() - 2), s); (yyval.shval) = s; }
    break;

  case 24:

/* Line 677 of lalr1.cc  */
#line 177 "../src/scene/sceneparser.y"
    { (yysemantic_stack_[(2) - (2)].listval)->add((yysemantic_stack_[(2) - (1)].pval)); (yyval.listval) = (yysemantic_stack_[(2) - (2)].listval); }
    break;

  case 25:

/* Line 677 of lalr1.cc  */
#line 178 "../src/scene/sceneparser.y"
    { arraylist<primitive*>* l = new arraylist<primitive*>(); l->add((yysemantic_stack_[(1) - (1)].pval)); (yyval.listval) = l; }
    break;

  case 26:

/* Line 677 of lalr1.cc  */
#line 182 "../src/scene/sceneparser.y"
    { (yyval.pval) = (yysemantic_stack_[(1) - (1)].pval); }
    break;

  case 27:

/* Line 677 of lalr1.cc  */
#line 183 "../src/scene/sceneparser.y"
    { (yyval.pval) = (yysemantic_stack_[(1) - (1)].pval); }
    break;

  case 28:

/* Line 677 of lalr1.cc  */
#line 188 "../src/scene/sceneparser.y"
    { (yyval.pval) = new triangle(point3((yysemantic_stack_[(20) - (3)].fval), (yysemantic_stack_[(20) - (5)].fval), (yysemantic_stack_[(20) - (7)].fval)), point3((yysemantic_stack_[(20) - (9)].fval), (yysemantic_stack_[(20) - (11)].fval), (yysemantic_stack_[(20) - (13)].fval)), point3((yysemantic_stack_[(20) - (15)].fval), (yysemantic_stack_[(20) - (17)].fval), (yysemantic_stack_[(20) - (19)].fval))); }
    break;

  case 29:

/* Line 677 of lalr1.cc  */
#line 193 "../src/scene/sceneparser.y"
    { (yyval.pval) = new sphere(point3((yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (7)].fval)), (yysemantic_stack_[(10) - (9)].fval)); }
    break;

  case 30:

/* Line 677 of lalr1.cc  */
#line 197 "../src/scene/sceneparser.y"
    { bsdfPtr p((yysemantic_stack_[(4) - (3)].bval)); material* mat = new material(p); (yyval.mval) = mat; }
    break;

  case 31:

/* Line 677 of lalr1.cc  */
#line 199 "../src/scene/sceneparser.y"
    { (yyval.mval) = new material(rgbColor((yysemantic_stack_[(13) - (6)].fval), (yysemantic_stack_[(13) - (8)].fval), (yysemantic_stack_[(13) - (10)].fval)), (yysemantic_stack_[(13) - (12)].fval)); }
    break;

  case 32:

/* Line 677 of lalr1.cc  */
#line 203 "../src/scene/sceneparser.y"
    { bsdf* b = new bsdf(); b->addBxdf((yysemantic_stack_[(1) - (1)].bxval)); (yyval.bval) = b; }
    break;

  case 33:

/* Line 677 of lalr1.cc  */
#line 204 "../src/scene/sceneparser.y"
    { bsdf* b = new bsdf(); b->addBxdf((yysemantic_stack_[(1) - (1)].bxval)); (yyval.bval) = b; }
    break;

  case 34:

/* Line 677 of lalr1.cc  */
#line 205 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 35:

/* Line 677 of lalr1.cc  */
#line 206 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 36:

/* Line 677 of lalr1.cc  */
#line 207 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 37:

/* Line 677 of lalr1.cc  */
#line 212 "../src/scene/sceneparser.y"
    { (yyval.bxval) = new lambertianBrdf(rgbColor((yysemantic_stack_[(8) - (3)].fval), (yysemantic_stack_[(8) - (5)].fval), (yysemantic_stack_[(8) - (7)].fval))); }
    break;

  case 38:

/* Line 677 of lalr1.cc  */
#line 217 "../src/scene/sceneparser.y"
    { (yyval.bxval) = new phongBrdf(rgbColor((yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (7)].fval)), (yysemantic_stack_[(10) - (9)].fval)); }
    break;

  case 39:

/* Line 677 of lalr1.cc  */
#line 222 "../src/scene/sceneparser.y"
    { (yyval.mdistval) = new blinn(rgbColor((yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (7)].fval)), (yysemantic_stack_[(10) - (9)].fval)); }
    break;

  case 40:

/* Line 677 of lalr1.cc  */
#line 227 "../src/scene/sceneparser.y"
    { (yyval.mdistval) = new blinn(rgbColor((yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (7)].fval)), (yysemantic_stack_[(10) - (9)].fval)); }
    break;

  case 41:

/* Line 677 of lalr1.cc  */
#line 232 "../src/scene/sceneparser.y"
    { (yyval.mdistval) = new aniso(rgbColor((yysemantic_stack_[(12) - (3)].fval), (yysemantic_stack_[(12) - (5)].fval), (yysemantic_stack_[(12) - (7)].fval)), (yysemantic_stack_[(12) - (9)].fval), (yysemantic_stack_[(12) - (11)].fval)); }
    break;

  case 42:

/* Line 677 of lalr1.cc  */
#line 235 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 43:

/* Line 677 of lalr1.cc  */
#line 236 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 44:

/* Line 677 of lalr1.cc  */
#line 241 "../src/scene/sceneparser.y"
    {
                     bsdf* p = new bsdf();
                     p->addBxdf(new specularBrdf((yysemantic_stack_[(13) - (12)].fval), 0.f, DIELECTRIC, rgbColor(1.f,1.f,1.f)));
                     p->addBxdf(new specularBtdf((yysemantic_stack_[(13) - (12)].fval), rgbColor((yysemantic_stack_[(13) - (6)].fval), (yysemantic_stack_[(13) - (8)].fval), (yysemantic_stack_[(13) - (10)].fval))));
                     (yyval.bval) = p;
                 }
    break;

  case 45:

/* Line 677 of lalr1.cc  */
#line 251 "../src/scene/sceneparser.y"
    {
                       bsdf* p = new bsdf();
                       p->addBxdf(new specularBrdf((yysemantic_stack_[(15) - (12)].fval), (yysemantic_stack_[(15) - (14)].fval), CONDUCTOR, rgbColor((yysemantic_stack_[(15) - (6)].fval), (yysemantic_stack_[(15) - (8)].fval), (yysemantic_stack_[(15) - (10)].fval))));
                       (yyval.bval) = p;
                   }
    break;

  case 46:

/* Line 677 of lalr1.cc  */
#line 259 "../src/scene/sceneparser.y"
    { (yyval.mdistval) = (yysemantic_stack_[(1) - (1)].mdistval); }
    break;

  case 47:

/* Line 677 of lalr1.cc  */
#line 260 "../src/scene/sceneparser.y"
    { (yyval.mdistval) = (yysemantic_stack_[(1) - (1)].mdistval); }
    break;

  case 48:

/* Line 677 of lalr1.cc  */
#line 261 "../src/scene/sceneparser.y"
    { (yyval.mdistval) = (yysemantic_stack_[(1) - (1)].mdistval); }
    break;

  case 49:

/* Line 677 of lalr1.cc  */
#line 266 "../src/scene/sceneparser.y"
    {
               (yyval.mbxval) = new microfacetBrdf((yysemantic_stack_[(8) - (3)].fval), (yysemantic_stack_[(8) - (5)].fval), (yysemantic_stack_[(8) - (7)].mdistval));
           }
    break;

  case 50:

/* Line 677 of lalr1.cc  */
#line 272 "../src/scene/sceneparser.y"
    { (yyval.bxval) = (yysemantic_stack_[(1) - (1)].mbxval); }
    break;

  case 51:

/* Line 677 of lalr1.cc  */
#line 273 "../src/scene/sceneparser.y"
    { (yyval.bxval) = (yysemantic_stack_[(1) - (1)].bxval); }
    break;

  case 52:

/* Line 677 of lalr1.cc  */
#line 274 "../src/scene/sceneparser.y"
    { (yyval.bxval) = (yysemantic_stack_[(1) - (1)].bxval); }
    break;

  case 53:

/* Line 677 of lalr1.cc  */
#line 279 "../src/scene/sceneparser.y"
    {
              bsdf* p = new bsdf();
              p->addBxdf(new substrate(rgbColor((yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (7)].fval)), (yysemantic_stack_[(10) - (9)].mdistval)->rho, (yysemantic_stack_[(10) - (9)].mdistval)));
              (yyval.bval) = p;
          }
    break;

  case 54:

/* Line 677 of lalr1.cc  */
#line 288 "../src/scene/sceneparser.y"
    {
         bsdf* p = new bsdf();
         p->addBxdf((yysemantic_stack_[(6) - (3)].bxval));
         p->addBxdf((yysemantic_stack_[(6) - (5)].bxval));
         (yyval.bval) = p;
     }
    break;



/* Line 677 of lalr1.cc  */
#line 785 "sceneparser.tab.c"
	default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (int yystate)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const short int Parser::yypact_ninf_ = -133;
  const short int
  Parser::yypact_[] =
  {
        18,     1,    28,     8,  -133,  -133,  -133,  -133,     3,     9,
    -133,  -133,  -133,     4,     2,    21,     6,     7,    11,    12,
      21,    21,  -133,  -133,    21,    22,    14,   -28,  -133,  -133,
    -133,  -133,    13,    15,    17,    27,    27,    25,  -133,  -133,
      20,    23,    24,    16,    26,     5,   -33,    19,    29,  -133,
      30,    31,    32,    33,    34,    36,    37,    39,   -13,  -133,
    -133,    38,    43,    44,    45,    46,    40,    41,    47,    42,
      48,    50,    49,    51,    52,  -133,  -133,  -133,  -133,  -133,
    -133,  -133,    54,    56,    59,    60,    61,    53,    64,    62,
      57,    66,    10,    67,   -18,  -133,    68,    70,    71,    72,
      73,    69,    74,    75,    76,    77,    79,    80,    78,    81,
    -133,  -133,  -133,    82,    83,    84,    86,    87,    88,  -133,
    -133,    89,    93,    94,    90,    91,    95,    96,   -18,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   110,   108,
     109,    97,   111,   112,   113,   114,    98,   115,   119,   120,
     118,   121,   122,   123,  -133,   125,   126,   127,   128,  -133,
     129,   130,   124,   131,   134,   132,   133,   135,   136,   137,
     138,   139,   143,  -133,   141,   142,    -5,    -5,   146,   147,
     148,   149,   150,   144,   151,   153,   152,   154,   155,  -133,
    -133,  -133,   156,   157,   160,   163,   164,   165,   161,  -133,
     167,   168,   158,   172,   173,  -133,  -133,   174,   175,   176,
     177,  -133,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   190,   192,   193,   194,   196,   197,   198,
    -133,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   210,   211,   212,   214,  -133,   216,  -133,   215,   217,
     218,   219,   220,   209,   224,   225,   226,   227,   228,  -133,
     221,   222,   229,   230,   231,  -133,  -133,   235,   236,   237,
     223,   238,   239,  -133,   240,   243,   241,   242,  -133,   244,
     245,   246,   247,  -133
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     0,     0,     0,     1,     3,     4,     5,     0,     0,
       6,     7,     8,     0,     0,     0,     0,     0,     0,     0,
      13,    14,    16,    17,    12,     0,     0,     0,     2,     9,
      10,    11,     0,     0,     0,     0,     0,    25,    26,    27,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    24,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    32,    33,    34,    42,    43,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,    52,    50,     0,     0,     0,     0,     0,     0,    22,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,     0,     0,     0,     0,    29,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    46,
      48,    47,     0,     0,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,    53,    49,     0,     0,     0,
       0,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    18,     0,    45,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    28,
       0,     0,     0,     0,     0,    39,    40,     0,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,    15,     0,
       0,     0,     0,    19
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
      -133,  -133,  -133,  -133,    -7,  -133,  -133,  -133,  -133,  -133,
    -133,   234,  -133,  -133,  -133,   248,  -133,   -10,    -8,  -133,
    -133,  -133,  -133,  -133,  -133,  -132,  -133,   -74,  -133,  -133
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,     2,     8,    13,    19,    20,    21,    22,    23,    24,
      35,    36,    37,    38,    39,    47,    74,   110,   111,   189,
     190,   191,    77,    78,    79,   192,   112,   113,    80,    81
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const unsigned short int
  Parser::yytable_[] =
  {
        69,    70,    57,    41,    42,    69,    70,    58,   109,    71,
      72,    73,   186,    29,    30,   187,   188,    31,     5,     6,
       7,     1,    10,    11,    12,    16,    17,    18,     4,    32,
      33,    34,    32,    33,    55,    56,     3,   106,   107,     9,
      15,    14,    26,    46,    25,   193,    27,    40,    75,    53,
      76,    28,    45,    43,   141,    44,    50,     0,    59,    54,
      51,    52,     0,    61,    68,     0,     0,     0,    60,    64,
      65,    62,    63,    66,    67,    82,    83,    84,    85,    86,
      87,    88,    90,     0,    89,    92,     0,   101,    91,    93,
     104,    94,    97,    95,    96,    98,    99,   100,   102,   105,
     108,   114,   103,   115,   116,   117,   118,     0,   121,     0,
     119,     0,   122,   123,   126,   120,   124,   125,   128,   129,
     130,   127,   131,   132,   133,   134,   135,   136,   139,   140,
     137,   138,   142,   143,   144,   145,   146,   147,   154,   159,
     150,   148,   149,   151,   152,   153,     0,   155,   156,   157,
     158,   160,   161,   162,   163,   165,   166,   164,   167,   168,
     169,   170,   171,     0,   174,   173,   172,   175,   176,   177,
       0,   178,   179,   180,   181,   182,   183,   184,   185,   194,
     195,   196,   197,   198,   200,   199,   201,     0,     0,     0,
       0,   214,   202,     0,   203,   204,   207,   205,   206,   208,
     209,   210,   211,   212,   213,   215,   216,   217,   218,   219,
     220,   221,   222,     0,     0,     0,   223,   224,   225,   226,
     227,   228,   229,   232,   231,   233,   234,   235,   230,   236,
     237,   238,   239,     0,     0,     0,   240,   241,   242,   243,
     244,   248,   246,   249,   250,   251,   245,   252,   247,   253,
     259,   254,     0,   255,   256,   257,   258,   260,   261,   262,
     263,   264,   265,   266,   273,   267,   268,   269,   270,   271,
     272,    49,     0,   276,   274,   275,   277,   280,   279,   282,
       0,   281,   278,     0,    48,     0,     0,     0,   283
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
        18,    19,    35,    31,    32,    18,    19,    40,    26,    22,
      23,    24,    17,    20,    21,    20,    21,    24,    10,    11,
      12,     3,    13,    14,    15,     4,     5,     6,     0,     7,
       8,     9,     7,     8,    29,    30,    35,    27,    28,    36,
      38,    37,    35,    16,    38,   177,    35,    33,    58,    33,
      58,    39,    35,    40,   128,    40,    36,    -1,    39,    33,
      37,    37,    -1,    33,    25,    -1,    -1,    -1,    39,    36,
      36,    40,    40,    37,    37,    37,    33,    33,    33,    33,
      40,    40,    40,    -1,    37,    35,    -1,    34,    40,    40,
      33,    40,    36,    41,    40,    36,    36,    36,    34,    33,
      33,    33,    40,    33,    33,    33,    33,    -1,    33,    -1,
      41,    -1,    36,    36,    36,    41,    37,    37,    36,    36,
      36,    40,    36,    36,    36,    36,    33,    33,    33,    33,
      40,    40,    33,    33,    33,    33,    33,    33,    41,    41,
      33,    36,    36,    33,    36,    36,    -1,    36,    36,    36,
      36,    36,    33,    33,    36,    33,    33,    36,    33,    33,
      33,    33,    33,    -1,    33,    41,    36,    33,    36,    36,
      -1,    36,    36,    36,    36,    36,    33,    36,    36,    33,
      33,    33,    33,    33,    33,    41,    33,    -1,    -1,    -1,
      -1,    33,    40,    -1,    40,    40,    36,    41,    41,    36,
      36,    36,    41,    36,    36,    33,    33,    33,    33,    33,
      33,    33,    33,    -1,    -1,    -1,    36,    36,    36,    36,
      36,    36,    36,    33,    36,    33,    33,    33,    41,    33,
      33,    33,    33,    -1,    -1,    -1,    36,    36,    36,    36,
      36,    33,    36,    33,    33,    33,    41,    33,    41,    33,
      41,    36,    -1,    36,    36,    36,    36,    33,    33,    33,
      33,    33,    41,    41,    41,    36,    36,    36,    33,    33,
      33,    37,    -1,    33,    36,    36,    33,    33,    36,    33,
      -1,    36,    41,    -1,    36,    -1,    -1,    -1,    41
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     3,    43,    35,     0,    10,    11,    12,    44,    36,
      13,    14,    15,    45,    37,    38,     4,     5,     6,    46,
      47,    48,    49,    50,    51,    38,    35,    35,    39,    46,
      46,    46,     7,     8,     9,    52,    53,    54,    55,    56,
      33,    31,    32,    40,    40,    35,    16,    57,    57,    53,
      36,    37,    37,    33,    33,    29,    30,    35,    40,    39,
      39,    33,    40,    40,    36,    36,    37,    37,    25,    18,
      19,    22,    23,    24,    58,    59,    60,    64,    65,    66,
      70,    71,    37,    33,    33,    33,    33,    40,    40,    37,
      40,    40,    35,    40,    40,    41,    40,    36,    36,    36,
      36,    34,    34,    40,    33,    33,    27,    28,    33,    26,
      59,    60,    68,    69,    33,    33,    33,    33,    33,    41,
      41,    33,    36,    36,    37,    37,    36,    40,    36,    36,
      36,    36,    36,    36,    36,    33,    33,    40,    40,    33,
      33,    69,    33,    33,    33,    33,    33,    33,    36,    36,
      33,    33,    36,    36,    41,    36,    36,    36,    36,    41,
      36,    33,    33,    36,    36,    33,    33,    33,    33,    33,
      33,    33,    36,    41,    33,    33,    36,    36,    36,    36,
      36,    36,    36,    33,    36,    36,    17,    20,    21,    61,
      62,    63,    67,    67,    33,    33,    33,    33,    33,    41,
      33,    33,    40,    40,    40,    41,    41,    36,    36,    36,
      36,    41,    36,    36,    33,    33,    33,    33,    33,    33,
      33,    33,    33,    36,    36,    36,    36,    36,    36,    36,
      41,    36,    33,    33,    33,    33,    33,    33,    33,    33,
      36,    36,    36,    36,    36,    41,    36,    41,    33,    33,
      33,    33,    33,    33,    36,    36,    36,    36,    36,    41,
      33,    33,    33,    33,    33,    41,    41,    36,    36,    36,
      33,    33,    33,    41,    36,    36,    33,    33,    41,    36,
      33,    36,    33,    41
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,    60,    44,    62,   123,   125,
      40,    41
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    42,    43,    44,    44,    44,    45,    45,    45,    46,
      46,    46,    46,    46,    46,    47,    48,    48,    49,    50,
      51,    51,    52,    52,    53,    53,    54,    54,    55,    56,
      57,    57,    58,    58,    58,    58,    58,    59,    60,    61,
      62,    63,    64,    64,    65,    66,    67,    67,    67,    68,
      69,    69,    69,    70,    71
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     9,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,    27,     1,     1,    19,    31,
       5,     5,     7,     7,     2,     1,     1,     1,    20,    10,
       4,    13,     1,     1,     1,     1,     1,     8,    10,    10,
      10,    12,     1,     1,    13,    15,     1,     1,     1,     8,
       1,     1,     1,    10,     6
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Parser::yytname_[] =
  {
    "$end", "error", "$undefined", "SCENE", "SHAPE", "CAMERA", "LIGHT",
  "TRIANGLE", "SPHERE", "OBJFILE", "BVH", "OCTREE", "DEFAULT", "WHITTED",
  "PATH", "BIDIR", "MATERIAL", "BLINN", "PHONG", "LAMBERT", "BECKMANN",
  "ANISO", "SPECULAR", "SUBSTRATE", "PAIR", "EMISSIVE", "MICROFACET",
  "DIELECTRIC", "CONDUCTOR", "SMOOTH", "FLAT", "AREATYPE", "POINTTYPE",
  "FLOAT", "FILEPATH", "'<'", "','", "'>'", "'{'", "'}'", "'('", "')'",
  "$accept", "scene_file", "accelerator", "tracer", "contents", "camera",
  "light", "pointlight", "arealight", "shape", "objfile", "primitive_list",
  "primitive", "triangle", "sphere", "material", "bsdf", "lambert",
  "phong", "blinn", "beckmann", "aniso", "specular", "specular_dielectric",
  "specular_conductor", "microfacetDistrib", "microfacet", "bxdf",
  "substrate", "pair", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        43,     0,    -1,     3,    35,    44,    36,    45,    37,    38,
      46,    39,    -1,    10,    -1,    11,    -1,    12,    -1,    13,
      -1,    14,    -1,    15,    -1,    47,    46,    -1,    48,    46,
      -1,    51,    46,    -1,    51,    -1,    47,    -1,    48,    -1,
       5,    35,    33,    36,    33,    37,    40,    33,    36,    33,
      36,    33,    36,    33,    36,    33,    36,    33,    36,    33,
      36,    33,    36,    33,    36,    33,    41,    -1,    49,    -1,
      50,    -1,     6,    35,    32,    37,    40,    33,    36,    33,
      36,    33,    36,    33,    36,    33,    36,    33,    36,    33,
      41,    -1,     6,    35,    31,    37,    40,    33,    36,    33,
      36,    33,    36,    33,    36,    33,    36,    33,    36,    33,
      36,    33,    36,    33,    36,    33,    36,    33,    36,    33,
      36,    33,    41,    -1,     4,    38,    53,    57,    39,    -1,
       4,    38,    52,    57,    39,    -1,     9,    35,    29,    37,
      40,    34,    41,    -1,     9,    35,    30,    37,    40,    34,
      41,    -1,    54,    53,    -1,    54,    -1,    55,    -1,    56,
      -1,     7,    40,    33,    36,    33,    36,    33,    36,    33,
      36,    33,    36,    33,    36,    33,    36,    33,    36,    33,
      41,    -1,     8,    40,    33,    36,    33,    36,    33,    36,
      33,    41,    -1,    16,    40,    58,    41,    -1,    16,    35,
      25,    37,    40,    33,    36,    33,    36,    33,    36,    33,
      41,    -1,    59,    -1,    60,    -1,    64,    -1,    70,    -1,
      71,    -1,    19,    40,    33,    36,    33,    36,    33,    41,
      -1,    18,    40,    33,    36,    33,    36,    33,    36,    33,
      41,    -1,    17,    40,    33,    36,    33,    36,    33,    36,
      33,    41,    -1,    20,    40,    33,    36,    33,    36,    33,
      36,    33,    41,    -1,    21,    40,    33,    36,    33,    36,
      33,    36,    33,    36,    33,    41,    -1,    65,    -1,    66,
      -1,    22,    35,    27,    37,    40,    33,    36,    33,    36,
      33,    36,    33,    41,    -1,    22,    35,    28,    37,    40,
      33,    36,    33,    36,    33,    36,    33,    36,    33,    41,
      -1,    61,    -1,    63,    -1,    62,    -1,    26,    40,    33,
      36,    33,    36,    67,    41,    -1,    68,    -1,    59,    -1,
      60,    -1,    23,    40,    33,    36,    33,    36,    33,    36,
      67,    41,    -1,    24,    40,    69,    36,    69,    41,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,    13,    15,    17,    19,    21,    23,    25,
      28,    31,    34,    36,    38,    40,    68,    70,    72,    92,
     124,   130,   136,   144,   152,   155,   157,   159,   161,   182,
     193,   198,   212,   214,   216,   218,   220,   222,   231,   242,
     253,   264,   277,   279,   281,   295,   311,   313,   315,   317,
     326,   328,   330,   332,   343
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   106,   106,   114,   115,   116,   120,   121,   122,   126,
     127,   128,   129,   129,   129,   133,   141,   142,   146,   151,
     155,   165,   169,   172,   177,   178,   182,   183,   187,   192,
     197,   198,   203,   204,   205,   206,   207,   211,   216,   221,
     226,   231,   235,   236,   240,   250,   259,   260,   261,   265,
     272,   273,   274,   278,   287
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      40,    41,     2,     2,    36,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      35,     2,    37,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    38,     2,    39,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 288;
  const int Parser::yynnts_ = 30;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 4;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 42;

  const unsigned int Parser::yyuser_token_number_max_ = 289;
  const Parser::token_number_type Parser::yyundef_token_ = 2;


/* Line 1053 of lalr1.cc  */
#line 4 "../src/scene/sceneparser.y"
} // Bison

/* Line 1053 of lalr1.cc  */
#line 1406 "sceneparser.tab.c"


/* Line 1055 of lalr1.cc  */
#line 295 "../src/scene/sceneparser.y"


void Bison::Parser::error(const Bison::Parser::location_type& loc, const std::string& msg){
    cerr << "Parse error at: " << loc << ": " << msg << endl;
}

#include "scanner.hpp"
static int yylex(Bison::Parser::semantic_type * yylval,
                 Bison::Parser::location_type * yylloc,
                 scanner &scan) {
    return scan.yylex(yylval, yylloc);
}

