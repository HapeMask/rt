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
#line 104 "../src/scene/sceneparser.y"
    {
                scn.setAccelerator(acceleratorPtr((yysemantic_stack_[(9) - (3)].aval)));
                scn.setTracer(rayTracerPtr((yysemantic_stack_[(9) - (5)].tval)));
            }
    break;

  case 3:

/* Line 677 of lalr1.cc  */
#line 111 "../src/scene/sceneparser.y"
    { (yyval.aval) = new bvh(); }
    break;

  case 4:

/* Line 677 of lalr1.cc  */
#line 112 "../src/scene/sceneparser.y"
    { (yyval.aval) = new octree(); }
    break;

  case 5:

/* Line 677 of lalr1.cc  */
#line 113 "../src/scene/sceneparser.y"
    { (yyval.aval) = new defaultAccelerator(); }
    break;

  case 6:

/* Line 677 of lalr1.cc  */
#line 117 "../src/scene/sceneparser.y"
    { (yyval.tval) = new whittedRayTracer(scn); }
    break;

  case 7:

/* Line 677 of lalr1.cc  */
#line 118 "../src/scene/sceneparser.y"
    { (yyval.tval) = new pathTracer(scn); }
    break;

  case 8:

/* Line 677 of lalr1.cc  */
#line 119 "../src/scene/sceneparser.y"
    { (yyval.tval) = new bdpt(scn); }
    break;

  case 15:

/* Line 677 of lalr1.cc  */
#line 131 "../src/scene/sceneparser.y"
    {
           cameraPtr cam(new camera((float)(yysemantic_stack_[(27) - (3)].fval), (float)(yysemantic_stack_[(27) - (5)].fval), 0.1f, 100.f, (yysemantic_stack_[(27) - (8)].fval), point3((yysemantic_stack_[(27) - (10)].fval), (yysemantic_stack_[(27) - (12)].fval), (yysemantic_stack_[(27) - (14)].fval)), point3((yysemantic_stack_[(27) - (16)].fval), (yysemantic_stack_[(27) - (18)].fval), (yysemantic_stack_[(27) - (20)].fval)), vec3((yysemantic_stack_[(27) - (22)].fval), (yysemantic_stack_[(27) - (24)].fval), (yysemantic_stack_[(27) - (26)].fval))));
           scn.setCamera(cam);
       }
    break;

  case 16:

/* Line 677 of lalr1.cc  */
#line 138 "../src/scene/sceneparser.y"
    { scn.addLight((yysemantic_stack_[(1) - (1)].lval)); }
    break;

  case 17:

/* Line 677 of lalr1.cc  */
#line 139 "../src/scene/sceneparser.y"
    { scn.addLight((yysemantic_stack_[(1) - (1)].lval)); }
    break;

  case 18:

/* Line 677 of lalr1.cc  */
#line 144 "../src/scene/sceneparser.y"
    { light* l = new pointLight(point3((yysemantic_stack_[(19) - (6)].fval), (yysemantic_stack_[(19) - (8)].fval), (yysemantic_stack_[(19) - (10)].fval)), (yysemantic_stack_[(19) - (18)].fval), rgbColor((yysemantic_stack_[(19) - (12)].fval), (yysemantic_stack_[(19) - (14)].fval), (yysemantic_stack_[(19) - (16)].fval))); (yyval.lval) = l; }
    break;

  case 19:

/* Line 677 of lalr1.cc  */
#line 149 "../src/scene/sceneparser.y"
    { light* l = new areaLight(point3((yysemantic_stack_[(31) - (6)].fval), (yysemantic_stack_[(31) - (8)].fval), (yysemantic_stack_[(31) - (10)].fval)), (yysemantic_stack_[(31) - (30)].fval), rgbColor((yysemantic_stack_[(31) - (24)].fval), (yysemantic_stack_[(31) - (26)].fval), (yysemantic_stack_[(31) - (28)].fval)), vec3((yysemantic_stack_[(31) - (12)].fval), (yysemantic_stack_[(31) - (14)].fval), (yysemantic_stack_[(31) - (16)].fval)), vec3((yysemantic_stack_[(31) - (18)].fval), (yysemantic_stack_[(31) - (20)].fval), (yysemantic_stack_[(31) - (22)].fval))); (yyval.lval) = l; }
    break;

  case 20:

/* Line 677 of lalr1.cc  */
#line 153 "../src/scene/sceneparser.y"
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
#line 162 "../src/scene/sceneparser.y"
    { (yysemantic_stack_[(5) - (3)].shval)->setMaterial((yysemantic_stack_[(5) - (4)].mval)); scn.addShape((yysemantic_stack_[(5) - (3)].shval)); }
    break;

  case 22:

/* Line 677 of lalr1.cc  */
#line 167 "../src/scene/sceneparser.y"
    { shape* s = new triangleMesh(); objParser::parse(std::string((yysemantic_stack_[(7) - (6)].sval)).substr(1, std::string((yysemantic_stack_[(7) - (6)].sval)).length() - 2), s); (yyval.shval) = s; }
    break;

  case 23:

/* Line 677 of lalr1.cc  */
#line 170 "../src/scene/sceneparser.y"
    { shape* s = new shape(); objParser::parse(std::string((yysemantic_stack_[(7) - (6)].sval)).substr(1, std::string((yysemantic_stack_[(7) - (6)].sval)).length() - 2), s); (yyval.shval) = s; }
    break;

  case 24:

/* Line 677 of lalr1.cc  */
#line 174 "../src/scene/sceneparser.y"
    { (yysemantic_stack_[(2) - (2)].listval)->add((yysemantic_stack_[(2) - (1)].pval)); (yyval.listval) = (yysemantic_stack_[(2) - (2)].listval); }
    break;

  case 25:

/* Line 677 of lalr1.cc  */
#line 175 "../src/scene/sceneparser.y"
    { arraylist<primitive*>* l = new arraylist<primitive*>(); l->add((yysemantic_stack_[(1) - (1)].pval)); (yyval.listval) = l; }
    break;

  case 26:

/* Line 677 of lalr1.cc  */
#line 179 "../src/scene/sceneparser.y"
    { (yyval.pval) = (yysemantic_stack_[(1) - (1)].pval); }
    break;

  case 27:

/* Line 677 of lalr1.cc  */
#line 180 "../src/scene/sceneparser.y"
    { (yyval.pval) = (yysemantic_stack_[(1) - (1)].pval); }
    break;

  case 28:

/* Line 677 of lalr1.cc  */
#line 185 "../src/scene/sceneparser.y"
    { (yyval.pval) = new triangle(point3((yysemantic_stack_[(20) - (3)].fval), (yysemantic_stack_[(20) - (5)].fval), (yysemantic_stack_[(20) - (7)].fval)), point3((yysemantic_stack_[(20) - (9)].fval), (yysemantic_stack_[(20) - (11)].fval), (yysemantic_stack_[(20) - (13)].fval)), point3((yysemantic_stack_[(20) - (15)].fval), (yysemantic_stack_[(20) - (17)].fval), (yysemantic_stack_[(20) - (19)].fval))); }
    break;

  case 29:

/* Line 677 of lalr1.cc  */
#line 190 "../src/scene/sceneparser.y"
    { (yyval.pval) = new sphere(point3((yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (7)].fval)), (yysemantic_stack_[(10) - (9)].fval)); }
    break;

  case 30:

/* Line 677 of lalr1.cc  */
#line 194 "../src/scene/sceneparser.y"
    { bsdfPtr p((yysemantic_stack_[(4) - (3)].bval)); material* mat = new material(p); (yyval.mval) = mat; }
    break;

  case 31:

/* Line 677 of lalr1.cc  */
#line 196 "../src/scene/sceneparser.y"
    { (yyval.mval) = new material(rgbColor((yysemantic_stack_[(13) - (6)].fval), (yysemantic_stack_[(13) - (8)].fval), (yysemantic_stack_[(13) - (10)].fval)), (yysemantic_stack_[(13) - (12)].fval)); }
    break;

  case 32:

/* Line 677 of lalr1.cc  */
#line 200 "../src/scene/sceneparser.y"
    { bsdf* b = new bsdf(); b->addBxdf((yysemantic_stack_[(1) - (1)].bxval)); (yyval.bval) = b; }
    break;

  case 33:

/* Line 677 of lalr1.cc  */
#line 201 "../src/scene/sceneparser.y"
    { bsdf* b = new bsdf(); b->addBxdf((yysemantic_stack_[(1) - (1)].bxval)); (yyval.bval) = b; }
    break;

  case 34:

/* Line 677 of lalr1.cc  */
#line 202 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 35:

/* Line 677 of lalr1.cc  */
#line 203 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 36:

/* Line 677 of lalr1.cc  */
#line 204 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 37:

/* Line 677 of lalr1.cc  */
#line 209 "../src/scene/sceneparser.y"
    { (yyval.bxval) = new lambertianBrdf(rgbColor((yysemantic_stack_[(8) - (3)].fval), (yysemantic_stack_[(8) - (5)].fval), (yysemantic_stack_[(8) - (7)].fval))); }
    break;

  case 38:

/* Line 677 of lalr1.cc  */
#line 214 "../src/scene/sceneparser.y"
    { (yyval.bxval) = new phongBrdf(rgbColor((yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (7)].fval)), (yysemantic_stack_[(10) - (9)].fval)); }
    break;

  case 39:

/* Line 677 of lalr1.cc  */
#line 219 "../src/scene/sceneparser.y"
    { (yyval.mbxval) = new blinnMicrofacet(rgbColor((yysemantic_stack_[(14) - (3)].fval), (yysemantic_stack_[(14) - (5)].fval), (yysemantic_stack_[(14) - (7)].fval)), (yysemantic_stack_[(14) - (9)].fval), (yysemantic_stack_[(14) - (11)].fval), (yysemantic_stack_[(14) - (13)].fval)); }
    break;

  case 40:

/* Line 677 of lalr1.cc  */
#line 224 "../src/scene/sceneparser.y"
    { (yyval.mbxval) = new asMicrofacet(rgbColor((yysemantic_stack_[(16) - (3)].fval), (yysemantic_stack_[(16) - (5)].fval), (yysemantic_stack_[(16) - (7)].fval)), (yysemantic_stack_[(16) - (9)].fval), (yysemantic_stack_[(16) - (11)].fval), (yysemantic_stack_[(16) - (13)].fval), (yysemantic_stack_[(16) - (15)].fval)); }
    break;

  case 41:

/* Line 677 of lalr1.cc  */
#line 227 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 42:

/* Line 677 of lalr1.cc  */
#line 228 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 43:

/* Line 677 of lalr1.cc  */
#line 233 "../src/scene/sceneparser.y"
    {
                     bsdf* p = new bsdf();
                     p->addBxdf(new specularBrdf((yysemantic_stack_[(13) - (12)].fval), 0.f, DIELECTRIC, rgbColor(1.f,1.f,1.f)));
                     p->addBxdf(new specularBtdf((yysemantic_stack_[(13) - (12)].fval), rgbColor((yysemantic_stack_[(13) - (6)].fval), (yysemantic_stack_[(13) - (8)].fval), (yysemantic_stack_[(13) - (10)].fval))));
                     (yyval.bval) = p;
                 }
    break;

  case 44:

/* Line 677 of lalr1.cc  */
#line 243 "../src/scene/sceneparser.y"
    {
                       bsdf* p = new bsdf();
                       p->addBxdf(new specularBrdf((yysemantic_stack_[(15) - (12)].fval), (yysemantic_stack_[(15) - (14)].fval), CONDUCTOR, rgbColor((yysemantic_stack_[(15) - (6)].fval), (yysemantic_stack_[(15) - (8)].fval), (yysemantic_stack_[(15) - (10)].fval))));
                       (yyval.bval) = p;
                   }
    break;

  case 45:

/* Line 677 of lalr1.cc  */
#line 251 "../src/scene/sceneparser.y"
    { (yyval.mbxval) = (yysemantic_stack_[(1) - (1)].mbxval); }
    break;

  case 46:

/* Line 677 of lalr1.cc  */
#line 252 "../src/scene/sceneparser.y"
    { (yyval.mbxval) = (yysemantic_stack_[(1) - (1)].mbxval); }
    break;

  case 47:

/* Line 677 of lalr1.cc  */
#line 256 "../src/scene/sceneparser.y"
    { (yyval.bxval) = (yysemantic_stack_[(1) - (1)].mbxval); }
    break;

  case 48:

/* Line 677 of lalr1.cc  */
#line 257 "../src/scene/sceneparser.y"
    { (yyval.bxval) = (yysemantic_stack_[(1) - (1)].bxval); }
    break;

  case 49:

/* Line 677 of lalr1.cc  */
#line 258 "../src/scene/sceneparser.y"
    { (yyval.bxval) = (yysemantic_stack_[(1) - (1)].bxval); }
    break;

  case 50:

/* Line 677 of lalr1.cc  */
#line 263 "../src/scene/sceneparser.y"
    {
              bsdf* p = new bsdf();
              p->addBxdf(new substrate(rgbColor((yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (7)].fval)), (yysemantic_stack_[(10) - (9)].mbxval)->rs(), (yysemantic_stack_[(10) - (9)].mbxval)));
              (yyval.bval) = p;
          }
    break;

  case 51:

/* Line 677 of lalr1.cc  */
#line 272 "../src/scene/sceneparser.y"
    {
         bsdf* p = new bsdf();
         p->addBxdf((yysemantic_stack_[(6) - (3)].bxval));
         p->addBxdf((yysemantic_stack_[(6) - (5)].bxval));
         (yyval.bval) = p;
     }
    break;



/* Line 677 of lalr1.cc  */
#line 762 "sceneparser.tab.c"
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
  const short int Parser::yypact_ninf_ = -135;
  const short int
  Parser::yypact_[] =
  {
         0,   -13,    16,     8,  -135,  -135,  -135,  -135,     1,     9,
    -135,  -135,  -135,    -8,     5,    21,     6,     3,    11,    12,
      21,    21,  -135,  -135,    21,    22,    15,     2,  -135,  -135,
    -135,  -135,     7,    13,    17,    28,    28,    27,  -135,  -135,
      18,    19,    20,    25,    26,    10,   -32,    23,    24,  -135,
      31,    29,    30,    32,    35,    36,    37,    34,   -18,  -135,
    -135,    38,    33,    39,    43,    44,    40,    41,    42,    45,
      46,    47,    48,    49,    50,  -135,  -135,  -135,  -135,  -135,
    -135,  -135,    52,    51,    54,    57,    58,    61,    62,    59,
      64,    65,    14,    67,    -9,  -135,    68,    69,    70,    71,
      72,    66,    73,    75,    74,    76,    78,    79,    77,    80,
      81,  -135,  -135,  -135,  -135,  -135,    82,    83,    86,    87,
      88,    89,  -135,  -135,    90,    84,    94,    91,    92,    95,
      96,    97,    -9,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   111,   109,   110,   112,    98,   113,   114,   115,
     116,    99,   117,   121,   122,   120,   123,   124,   125,   127,
    -135,   128,   129,   130,   131,  -135,   132,   133,   126,   135,
     137,   136,   138,   139,   140,   141,   142,   143,   144,   148,
    -135,   146,   147,    -6,   151,   152,   153,   154,   155,   156,
     157,   150,   159,   160,   158,   161,   162,   164,   165,   166,
     167,   163,  -135,   169,   170,  -135,   174,   175,   176,   177,
     178,   179,  -135,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   191,   193,   194,   195,   197,   198,  -135,
     199,   200,   201,   202,   203,   192,   204,   205,   208,   209,
     210,   211,  -135,   212,  -135,   206,   213,   214,   215,   207,
    -135,   219,   220,   221,  -135,   216,   222,   223,  -135,   227,
     228,   226,   229,   230,   231,   225,   232,  -135,   234,   233,
     237,   235,  -135
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
       0,     0,     0,     0,     0,    32,    33,    34,    41,    42,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    45,    46,    47,     0,     0,     0,     0,
       0,     0,    22,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,    29,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,     0,     0,    50,     0,     0,     0,     0,
       0,     0,    31,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    18,     0,    44,     0,     0,     0,     0,     0,
      39,     0,     0,     0,    28,     0,     0,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,    15,     0,     0,
       0,     0,    19
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
      -135,  -135,  -135,  -135,    -7,  -135,  -135,  -135,  -135,  -135,
    -135,   196,  -135,  -135,  -135,   134,  -135,   -10,    -4,  -135,
    -135,  -135,  -135,  -135,  -134,   -72,  -135,  -135
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  Parser::yydefgoto_[] =
  {
        -1,     2,     8,    13,    19,    20,    21,    22,    23,    24,
      35,    36,    37,    38,    39,    47,    74,   111,   112,   113,
     114,    77,    78,    79,   115,   116,    80,    81
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const unsigned short int
  Parser::yytable_[] =
  {
        69,    70,    57,     1,    71,    72,    73,    58,   109,    69,
      70,   109,   110,    29,    30,   110,     4,    31,     5,     6,
       7,     3,    10,    11,    12,    16,    17,    18,    14,    32,
      33,    34,    41,    42,    32,    33,     9,    26,    55,    56,
     106,   107,    15,    25,    46,    27,    43,    40,    75,   194,
      28,    45,    44,    50,    76,    51,    52,    53,    54,    68,
     146,    59,    60,    61,     0,    83,     0,    64,    62,    63,
      65,    84,    66,    67,    82,    85,    86,     0,    89,    87,
      88,    92,     0,     0,    90,    91,    97,    93,    94,    98,
      95,    96,    99,   100,   101,   102,   104,   105,   103,   108,
     117,   118,   119,   120,   121,     0,   122,   124,     0,   125,
       0,   126,   129,   123,   127,   128,   139,   132,   133,   130,
     131,   134,   135,   136,   137,   138,   140,   143,   144,   145,
     141,   142,   147,   148,   149,   150,   151,   152,   160,   165,
     155,   153,   154,   156,   157,   158,     0,   159,   161,   162,
     163,   164,   166,   167,   168,   169,   171,   172,   170,   173,
     174,   175,   176,   177,   178,     0,   180,   181,   179,   182,
      48,   183,     0,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,     0,     0,     0,   206,   207,   205,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,     0,     0,     0,   223,   224,   225,
     226,   227,   228,   231,   230,   232,   233,   234,   229,   235,
     236,   237,   242,    49,     0,   238,   239,   240,   241,   243,
     245,   246,   247,   248,   249,   244,   250,   254,   251,   252,
     253,   255,   256,   257,     0,     0,   258,   259,   260,   261,
     262,   263,   265,   266,   264,   267,   269,   268,   270,   271,
       0,     0,     0,     0,     0,   272
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
        18,    19,    34,     3,    22,    23,    24,    39,    17,    18,
      19,    17,    21,    20,    21,    21,     0,    24,    10,    11,
      12,    34,    13,    14,    15,     4,     5,     6,    36,     7,
       8,     9,    30,    31,     7,     8,    35,    34,    28,    29,
      26,    27,    37,    37,    16,    34,    39,    32,    58,   183,
      38,    34,    39,    35,    58,    36,    36,    32,    32,    25,
     132,    38,    38,    32,    -1,    32,    -1,    35,    39,    39,
      35,    32,    36,    36,    36,    32,    32,    -1,    36,    39,
      39,    34,    -1,    -1,    39,    39,    35,    39,    39,    35,
      40,    39,    35,    35,    33,    33,    32,    32,    39,    32,
      32,    32,    32,    32,    32,    -1,    40,    32,    -1,    35,
      -1,    35,    35,    40,    36,    36,    32,    35,    35,    39,
      39,    35,    35,    35,    35,    35,    32,    32,    32,    32,
      39,    39,    32,    32,    32,    32,    32,    32,    40,    40,
      32,    35,    35,    32,    35,    35,    -1,    35,    35,    35,
      35,    35,    35,    32,    32,    35,    32,    32,    35,    32,
      32,    32,    32,    32,    32,    -1,    40,    32,    35,    32,
      36,    35,    -1,    35,    35,    35,    35,    35,    35,    35,
      32,    35,    35,    32,    32,    32,    32,    32,    32,    32,
      40,    32,    32,    -1,    -1,    -1,    35,    35,    40,    35,
      35,    35,    35,    40,    35,    35,    32,    32,    32,    32,
      32,    32,    32,    32,    -1,    -1,    -1,    35,    35,    35,
      35,    35,    35,    32,    35,    32,    32,    32,    40,    32,
      32,    32,    40,    37,    -1,    35,    35,    35,    35,    35,
      32,    32,    32,    32,    32,    40,    40,    40,    35,    35,
      35,    32,    32,    32,    -1,    -1,    40,    35,    35,    32,
      32,    35,    32,    32,    35,    40,    32,    35,    35,    32,
      -1,    -1,    -1,    -1,    -1,    40
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     3,    42,    34,     0,    10,    11,    12,    43,    35,
      13,    14,    15,    44,    36,    37,     4,     5,     6,    45,
      46,    47,    48,    49,    50,    37,    34,    34,    38,    45,
      45,    45,     7,     8,     9,    51,    52,    53,    54,    55,
      32,    30,    31,    39,    39,    34,    16,    56,    56,    52,
      35,    36,    36,    32,    32,    28,    29,    34,    39,    38,
      38,    32,    39,    39,    35,    35,    36,    36,    25,    18,
      19,    22,    23,    24,    57,    58,    59,    62,    63,    64,
      67,    68,    36,    32,    32,    32,    32,    39,    39,    36,
      39,    39,    34,    39,    39,    40,    39,    35,    35,    35,
      35,    33,    33,    39,    32,    32,    26,    27,    32,    17,
      21,    58,    59,    60,    61,    65,    66,    32,    32,    32,
      32,    32,    40,    40,    32,    35,    35,    36,    36,    35,
      39,    39,    35,    35,    35,    35,    35,    35,    35,    32,
      32,    39,    39,    32,    32,    32,    66,    32,    32,    32,
      32,    32,    32,    35,    35,    32,    32,    35,    35,    35,
      40,    35,    35,    35,    35,    40,    35,    32,    32,    35,
      35,    32,    32,    32,    32,    32,    32,    32,    32,    35,
      40,    32,    32,    35,    35,    35,    35,    35,    35,    35,
      35,    32,    35,    35,    65,    32,    32,    32,    32,    32,
      32,    32,    40,    32,    32,    40,    35,    35,    35,    35,
      35,    35,    40,    35,    35,    32,    32,    32,    32,    32,
      32,    32,    32,    35,    35,    35,    35,    35,    35,    40,
      35,    32,    32,    32,    32,    32,    32,    32,    35,    35,
      35,    35,    40,    35,    40,    32,    32,    32,    32,    32,
      40,    35,    35,    35,    40,    32,    32,    32,    40,    35,
      35,    32,    32,    35,    35,    32,    32,    40,    35,    32,
      35,    32,    40
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
     285,   286,   287,   288,    60,    44,    62,   123,   125,    40,
      41
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    41,    42,    43,    43,    43,    44,    44,    44,    45,
      45,    45,    45,    45,    45,    46,    47,    47,    48,    49,
      50,    50,    51,    51,    52,    52,    53,    53,    54,    55,
      56,    56,    57,    57,    57,    57,    57,    58,    59,    60,
      61,    62,    62,    63,    64,    65,    65,    66,    66,    66,
      67,    68
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     9,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,    27,     1,     1,    19,    31,
       5,     5,     7,     7,     2,     1,     1,     1,    20,    10,
       4,    13,     1,     1,     1,     1,     1,     8,    10,    14,
      16,     1,     1,    13,    15,     1,     1,     1,     1,     1,
      10,     6
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
  "ANISO", "SPECULAR", "SUBSTRATE", "PAIR", "EMISSIVE", "DIELECTRIC",
  "CONDUCTOR", "SMOOTH", "FLAT", "AREATYPE", "POINTTYPE", "FLOAT",
  "FILEPATH", "'<'", "','", "'>'", "'{'", "'}'", "'('", "')'", "$accept",
  "scene_file", "accelerator", "tracer", "contents", "camera", "light",
  "pointlight", "arealight", "shape", "objfile", "primitive_list",
  "primitive", "triangle", "sphere", "material", "bsdf", "lambert",
  "phong", "blinn", "aniso", "specular", "specular_dielectric",
  "specular_conductor", "microfacet", "bxdf", "substrate", "pair", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        42,     0,    -1,     3,    34,    43,    35,    44,    36,    37,
      45,    38,    -1,    10,    -1,    11,    -1,    12,    -1,    13,
      -1,    14,    -1,    15,    -1,    46,    45,    -1,    47,    45,
      -1,    50,    45,    -1,    50,    -1,    46,    -1,    47,    -1,
       5,    34,    32,    35,    32,    36,    39,    32,    35,    32,
      35,    32,    35,    32,    35,    32,    35,    32,    35,    32,
      35,    32,    35,    32,    35,    32,    40,    -1,    48,    -1,
      49,    -1,     6,    34,    31,    36,    39,    32,    35,    32,
      35,    32,    35,    32,    35,    32,    35,    32,    35,    32,
      40,    -1,     6,    34,    30,    36,    39,    32,    35,    32,
      35,    32,    35,    32,    35,    32,    35,    32,    35,    32,
      35,    32,    35,    32,    35,    32,    35,    32,    35,    32,
      35,    32,    40,    -1,     4,    37,    52,    56,    38,    -1,
       4,    37,    51,    56,    38,    -1,     9,    34,    28,    36,
      39,    33,    40,    -1,     9,    34,    29,    36,    39,    33,
      40,    -1,    53,    52,    -1,    53,    -1,    54,    -1,    55,
      -1,     7,    39,    32,    35,    32,    35,    32,    35,    32,
      35,    32,    35,    32,    35,    32,    35,    32,    35,    32,
      40,    -1,     8,    39,    32,    35,    32,    35,    32,    35,
      32,    40,    -1,    16,    39,    57,    40,    -1,    16,    34,
      25,    36,    39,    32,    35,    32,    35,    32,    35,    32,
      40,    -1,    58,    -1,    59,    -1,    62,    -1,    67,    -1,
      68,    -1,    19,    39,    32,    35,    32,    35,    32,    40,
      -1,    18,    39,    32,    35,    32,    35,    32,    35,    32,
      40,    -1,    17,    39,    32,    35,    32,    35,    32,    35,
      32,    35,    32,    35,    32,    40,    -1,    21,    39,    32,
      35,    32,    35,    32,    35,    32,    35,    32,    35,    32,
      35,    32,    40,    -1,    63,    -1,    64,    -1,    22,    34,
      26,    36,    39,    32,    35,    32,    35,    32,    35,    32,
      40,    -1,    22,    34,    27,    36,    39,    32,    35,    32,
      35,    32,    35,    32,    35,    32,    40,    -1,    60,    -1,
      61,    -1,    65,    -1,    58,    -1,    59,    -1,    23,    39,
      32,    35,    32,    35,    32,    35,    65,    40,    -1,    24,
      39,    66,    35,    66,    40,    -1
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
     257,   274,   276,   278,   292,   308,   310,   312,   314,   316,
     318,   329
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   103,   103,   111,   112,   113,   117,   118,   119,   123,
     124,   125,   126,   126,   126,   130,   138,   139,   143,   148,
     152,   162,   166,   169,   174,   175,   179,   180,   184,   189,
     194,   195,   200,   201,   202,   203,   204,   208,   213,   218,
     223,   227,   228,   232,   242,   251,   252,   256,   257,   258,
     262,   271
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
      39,    40,     2,     2,    35,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      34,     2,    36,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,    38,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 275;
  const int Parser::yynnts_ = 28;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 4;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 41;

  const unsigned int Parser::yyuser_token_number_max_ = 288;
  const Parser::token_number_type Parser::yyundef_token_ = 2;


/* Line 1053 of lalr1.cc  */
#line 4 "../src/scene/sceneparser.y"
} // Bison

/* Line 1053 of lalr1.cc  */
#line 1376 "sceneparser.tab.c"


/* Line 1055 of lalr1.cc  */
#line 279 "../src/scene/sceneparser.y"


void Bison::Parser::error(const Bison::Parser::location_type& loc, const std::string& msg){
    cerr << "Parse error at: " << loc << ": " << msg << endl;
}

#include "scanner.hpp"
static int yylex(Bison::Parser::semantic_type * yylval,
                 Bison::Parser::location_type * yylloc,
                 scanner &scan) {
    return scan.yylex(yylval, yylloc);
}

