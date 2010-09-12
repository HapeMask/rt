/* A Bison parser, made by GNU Bison 2.4.3.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010 Free
   Software Foundation, Inc.
   
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


/* Line 311 of lalr1.cc  */
#line 40 "sceneparser.tab.c"


#include "sceneparser.tab.h"

/* User implementation prologue.  */


/* Line 317 of lalr1.cc  */
#line 49 "sceneparser.tab.c"
/* Unqualified %code blocks.  */

/* Line 318 of lalr1.cc  */
#line 41 "../src/scene/sceneparser.y"

    // Prototype for the yylex function
    static int yylex(Bison::Parser::semantic_type * yylval,
                     Bison::Parser::location_type * yylloc,
                     scanner &scan);



/* Line 318 of lalr1.cc  */
#line 63 "sceneparser.tab.c"

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


/* Line 380 of lalr1.cc  */
#line 4 "../src/scene/sceneparser.y"
namespace Bison {

/* Line 380 of lalr1.cc  */
#line 131 "sceneparser.tab.c"
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
    location_type yyerror_range[3];

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

/* Line 678 of lalr1.cc  */
#line 108 "../src/scene/sceneparser.y"
    {
                scn.setAccelerator(acceleratorPtr((yysemantic_stack_[(9) - (3)].aval)));
                scn.setTracer(rayTracerPtr((yysemantic_stack_[(9) - (5)].tval)));
            }
    break;

  case 3:

/* Line 678 of lalr1.cc  */
#line 115 "../src/scene/sceneparser.y"
    { (yyval.aval) = new bvh(); }
    break;

  case 4:

/* Line 678 of lalr1.cc  */
#line 116 "../src/scene/sceneparser.y"
    { (yyval.aval) = new octree(); }
    break;

  case 5:

/* Line 678 of lalr1.cc  */
#line 117 "../src/scene/sceneparser.y"
    { (yyval.aval) = new defaultAccelerator(); }
    break;

  case 6:

/* Line 678 of lalr1.cc  */
#line 121 "../src/scene/sceneparser.y"
    { (yyval.tval) = new whittedRayTracer(scn); }
    break;

  case 7:

/* Line 678 of lalr1.cc  */
#line 122 "../src/scene/sceneparser.y"
    { (yyval.tval) = new pathTracer(scn); }
    break;

  case 8:

/* Line 678 of lalr1.cc  */
#line 123 "../src/scene/sceneparser.y"
    { (yyval.tval) = new bdpt(scn); }
    break;

  case 15:

/* Line 678 of lalr1.cc  */
#line 135 "../src/scene/sceneparser.y"
    {
           cameraPtr cam(new camera((float)(yysemantic_stack_[(27) - (3)].fval), (float)(yysemantic_stack_[(27) - (5)].fval), 0.1f, 100.f, (yysemantic_stack_[(27) - (8)].fval), point3((yysemantic_stack_[(27) - (10)].fval), (yysemantic_stack_[(27) - (12)].fval), (yysemantic_stack_[(27) - (14)].fval)), point3((yysemantic_stack_[(27) - (16)].fval), (yysemantic_stack_[(27) - (18)].fval), (yysemantic_stack_[(27) - (20)].fval)), vec3((yysemantic_stack_[(27) - (22)].fval), (yysemantic_stack_[(27) - (24)].fval), (yysemantic_stack_[(27) - (26)].fval))));
           scn.setCamera(cam);
       }
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 142 "../src/scene/sceneparser.y"
    { scn.addLight((yysemantic_stack_[(1) - (1)].lval)); }
    break;

  case 17:

/* Line 678 of lalr1.cc  */
#line 143 "../src/scene/sceneparser.y"
    { scn.addLight((yysemantic_stack_[(1) - (1)].lval)); }
    break;

  case 18:

/* Line 678 of lalr1.cc  */
#line 148 "../src/scene/sceneparser.y"
    { light* l = new pointLight(point3((yysemantic_stack_[(19) - (6)].fval), (yysemantic_stack_[(19) - (8)].fval), (yysemantic_stack_[(19) - (10)].fval)), (yysemantic_stack_[(19) - (18)].fval), rgbColor((yysemantic_stack_[(19) - (12)].fval), (yysemantic_stack_[(19) - (14)].fval), (yysemantic_stack_[(19) - (16)].fval))); (yyval.lval) = l; }
    break;

  case 19:

/* Line 678 of lalr1.cc  */
#line 153 "../src/scene/sceneparser.y"
    { light* l = new areaLight(point3((yysemantic_stack_[(31) - (6)].fval), (yysemantic_stack_[(31) - (8)].fval), (yysemantic_stack_[(31) - (10)].fval)), (yysemantic_stack_[(31) - (30)].fval), rgbColor((yysemantic_stack_[(31) - (24)].fval), (yysemantic_stack_[(31) - (26)].fval), (yysemantic_stack_[(31) - (28)].fval)), vec3((yysemantic_stack_[(31) - (12)].fval), (yysemantic_stack_[(31) - (14)].fval), (yysemantic_stack_[(31) - (16)].fval)), vec3((yysemantic_stack_[(31) - (18)].fval), (yysemantic_stack_[(31) - (20)].fval), (yysemantic_stack_[(31) - (22)].fval))); (yyval.lval) = l; }
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 157 "../src/scene/sceneparser.y"
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

/* Line 678 of lalr1.cc  */
#line 166 "../src/scene/sceneparser.y"
    { (yysemantic_stack_[(5) - (3)].shval)->setMaterial((yysemantic_stack_[(5) - (4)].mval)); scn.addShape((yysemantic_stack_[(5) - (3)].shval)); }
    break;

  case 22:

/* Line 678 of lalr1.cc  */
#line 171 "../src/scene/sceneparser.y"
    { triangleMesh* p = new triangleMesh(); objParser::parse(std::string((yysemantic_stack_[(7) - (6)].sval)).substr(1, std::string((yysemantic_stack_[(7) - (6)].sval)).length() - 2), p); (yyval.shval) = p; }
    break;

  case 23:

/* Line 678 of lalr1.cc  */
#line 174 "../src/scene/sceneparser.y"
    { shape* s = new shape(); objParser::parse(std::string((yysemantic_stack_[(7) - (6)].sval)).substr(1, std::string((yysemantic_stack_[(7) - (6)].sval)).length() - 2), s); (yyval.shval) = s; }
    break;

  case 24:

/* Line 678 of lalr1.cc  */
#line 178 "../src/scene/sceneparser.y"
    { (yysemantic_stack_[(2) - (2)].listval)->add((yysemantic_stack_[(2) - (1)].pval)); (yyval.listval) = (yysemantic_stack_[(2) - (2)].listval); }
    break;

  case 25:

/* Line 678 of lalr1.cc  */
#line 179 "../src/scene/sceneparser.y"
    { arraylist<primitive*>* l = new arraylist<primitive*>(); l->add((yysemantic_stack_[(1) - (1)].pval)); (yyval.listval) = l; }
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 183 "../src/scene/sceneparser.y"
    { (yyval.pval) = (yysemantic_stack_[(1) - (1)].pval); }
    break;

  case 27:

/* Line 678 of lalr1.cc  */
#line 184 "../src/scene/sceneparser.y"
    { (yyval.pval) = (yysemantic_stack_[(1) - (1)].pval); }
    break;

  case 28:

/* Line 678 of lalr1.cc  */
#line 189 "../src/scene/sceneparser.y"
    { (yyval.pval) = new triangle(point3((yysemantic_stack_[(20) - (3)].fval), (yysemantic_stack_[(20) - (5)].fval), (yysemantic_stack_[(20) - (7)].fval)), point3((yysemantic_stack_[(20) - (9)].fval), (yysemantic_stack_[(20) - (11)].fval), (yysemantic_stack_[(20) - (13)].fval)), point3((yysemantic_stack_[(20) - (15)].fval), (yysemantic_stack_[(20) - (17)].fval), (yysemantic_stack_[(20) - (19)].fval))); }
    break;

  case 29:

/* Line 678 of lalr1.cc  */
#line 194 "../src/scene/sceneparser.y"
    { (yyval.pval) = new sphere(point3((yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (7)].fval)), (yysemantic_stack_[(10) - (9)].fval)); }
    break;

  case 30:

/* Line 678 of lalr1.cc  */
#line 198 "../src/scene/sceneparser.y"
    { bsdfPtr p((yysemantic_stack_[(4) - (3)].bval)); material* mat = new material(p); (yyval.mval) = mat; }
    break;

  case 31:

/* Line 678 of lalr1.cc  */
#line 200 "../src/scene/sceneparser.y"
    { (yyval.mval) = new material(rgbColor((yysemantic_stack_[(13) - (6)].fval), (yysemantic_stack_[(13) - (8)].fval), (yysemantic_stack_[(13) - (10)].fval)), (yysemantic_stack_[(13) - (12)].fval)); }
    break;

  case 32:

/* Line 678 of lalr1.cc  */
#line 204 "../src/scene/sceneparser.y"
    { bsdf* b = new bsdf(); b->addBxdf((yysemantic_stack_[(1) - (1)].bxval)); (yyval.bval) = b; }
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 205 "../src/scene/sceneparser.y"
    { bsdf* b = new bsdf(); b->addBxdf((yysemantic_stack_[(1) - (1)].bxval)); (yyval.bval) = b; }
    break;

  case 34:

/* Line 678 of lalr1.cc  */
#line 206 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 207 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 208 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 213 "../src/scene/sceneparser.y"
    { (yyval.bxval) = new lambertianBrdf(rgbColor((yysemantic_stack_[(8) - (3)].fval), (yysemantic_stack_[(8) - (5)].fval), (yysemantic_stack_[(8) - (7)].fval))); }
    break;

  case 38:

/* Line 678 of lalr1.cc  */
#line 218 "../src/scene/sceneparser.y"
    { (yyval.bxval) = new phongBrdf(rgbColor((yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (7)].fval)), (yysemantic_stack_[(10) - (9)].fval)); }
    break;

  case 39:

/* Line 678 of lalr1.cc  */
#line 225 "../src/scene/sceneparser.y"
    { (yyval.bxval) = new newWard(rgbColor((yysemantic_stack_[(12) - (3)].fval), (yysemantic_stack_[(12) - (5)].fval), (yysemantic_stack_[(12) - (7)].fval)), 1.f/(yysemantic_stack_[(12) - (9)].fval), 1.f/(yysemantic_stack_[(12) - (11)].fval)); }
    break;

  case 40:

/* Line 678 of lalr1.cc  */
#line 230 "../src/scene/sceneparser.y"
    { (yyval.mdistval) = new blinn(rgbColor((yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (7)].fval)), (yysemantic_stack_[(10) - (9)].fval)); }
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 235 "../src/scene/sceneparser.y"
    { (yyval.mdistval) = new blinn(rgbColor((yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (7)].fval)), (yysemantic_stack_[(10) - (9)].fval)); }
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 240 "../src/scene/sceneparser.y"
    { (yyval.mdistval) = new aniso(rgbColor((yysemantic_stack_[(12) - (3)].fval), (yysemantic_stack_[(12) - (5)].fval), (yysemantic_stack_[(12) - (7)].fval)), (yysemantic_stack_[(12) - (9)].fval), (yysemantic_stack_[(12) - (11)].fval)); }
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 243 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 244 "../src/scene/sceneparser.y"
    { (yyval.bval) = (yysemantic_stack_[(1) - (1)].bval); }
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 249 "../src/scene/sceneparser.y"
    {
                     bsdf* p = new bsdf();
                     p->addBxdf(new specularBrdf((yysemantic_stack_[(13) - (12)].fval), 0.f, DIELECTRIC, rgbColor(1.f,1.f,1.f)));
                     p->addBxdf(new specularBtdf((yysemantic_stack_[(13) - (12)].fval), rgbColor((yysemantic_stack_[(13) - (6)].fval), (yysemantic_stack_[(13) - (8)].fval), (yysemantic_stack_[(13) - (10)].fval))));
                     (yyval.bval) = p;
                 }
    break;

  case 46:

/* Line 678 of lalr1.cc  */
#line 259 "../src/scene/sceneparser.y"
    {
                       bsdf* p = new bsdf();
                       p->addBxdf(new specularBrdf((yysemantic_stack_[(15) - (12)].fval), (yysemantic_stack_[(15) - (14)].fval), CONDUCTOR, rgbColor((yysemantic_stack_[(15) - (6)].fval), (yysemantic_stack_[(15) - (8)].fval), (yysemantic_stack_[(15) - (10)].fval))));
                       (yyval.bval) = p;
                   }
    break;

  case 47:

/* Line 678 of lalr1.cc  */
#line 267 "../src/scene/sceneparser.y"
    { (yyval.mdistval) = (yysemantic_stack_[(1) - (1)].mdistval); }
    break;

  case 48:

/* Line 678 of lalr1.cc  */
#line 268 "../src/scene/sceneparser.y"
    { (yyval.mdistval) = (yysemantic_stack_[(1) - (1)].mdistval); }
    break;

  case 49:

/* Line 678 of lalr1.cc  */
#line 269 "../src/scene/sceneparser.y"
    { (yyval.mdistval) = (yysemantic_stack_[(1) - (1)].mdistval); }
    break;

  case 50:

/* Line 678 of lalr1.cc  */
#line 274 "../src/scene/sceneparser.y"
    {
               (yyval.mbxval) = new microfacetBrdf((yysemantic_stack_[(8) - (3)].fval), (yysemantic_stack_[(8) - (5)].fval), (yysemantic_stack_[(8) - (7)].mdistval));
           }
    break;

  case 51:

/* Line 678 of lalr1.cc  */
#line 280 "../src/scene/sceneparser.y"
    { (yyval.bxval) = (yysemantic_stack_[(1) - (1)].mbxval); }
    break;

  case 52:

/* Line 678 of lalr1.cc  */
#line 281 "../src/scene/sceneparser.y"
    { (yyval.bxval) = (yysemantic_stack_[(1) - (1)].bxval); }
    break;

  case 53:

/* Line 678 of lalr1.cc  */
#line 282 "../src/scene/sceneparser.y"
    { (yyval.bxval) = (yysemantic_stack_[(1) - (1)].bxval); }
    break;

  case 54:

/* Line 678 of lalr1.cc  */
#line 283 "../src/scene/sceneparser.y"
    { (yyval.bxval) = (yysemantic_stack_[(1) - (1)].bxval); }
    break;

  case 55:

/* Line 678 of lalr1.cc  */
#line 288 "../src/scene/sceneparser.y"
    {
              bsdf* p = new bsdf();
              p->addBxdf(new substrate(rgbColor((yysemantic_stack_[(10) - (3)].fval), (yysemantic_stack_[(10) - (5)].fval), (yysemantic_stack_[(10) - (7)].fval)), (yysemantic_stack_[(10) - (9)].mdistval)->rho, (yysemantic_stack_[(10) - (9)].mdistval)));
              (yyval.bval) = p;
          }
    break;

  case 56:

/* Line 678 of lalr1.cc  */
#line 297 "../src/scene/sceneparser.y"
    {
         bsdf* p = new bsdf();
         p->addBxdf((yysemantic_stack_[(6) - (3)].bxval));
         p->addBxdf((yysemantic_stack_[(6) - (5)].bxval));
         (yyval.bval) = p;
     }
    break;



/* Line 678 of lalr1.cc  */
#line 800 "sceneparser.tab.c"
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

    yyerror_range[1] = yylloc;
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

    yyerror_range[1] = yylocation_stack_[yylen - 1];
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

	yyerror_range[1] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[2] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
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
  const short int Parser::yypact_ninf_ = -138;
  const short int
  Parser::yypact_[] =
  {
         2,   -19,    32,    -8,  -138,  -138,  -138,  -138,     3,    13,
    -138,  -138,  -138,     4,     5,    25,     8,     9,    12,    11,
      25,    25,  -138,  -138,    25,    26,    15,    -9,  -138,  -138,
    -138,  -138,     0,    14,    16,    27,    27,     7,  -138,  -138,
      17,    18,    19,    24,    28,     6,   -23,    20,    21,  -138,
      29,    23,    30,    22,    31,    34,    35,    40,   -12,  -138,
    -138,    36,    33,    41,    42,    43,    37,    38,    44,    39,
      45,    47,    46,    48,    49,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,    51,    53,    56,    57,    58,    50,    61,    59,
      54,    63,    10,    64,   -18,  -138,    65,    67,    68,    69,
      70,    66,    71,    72,    73,    74,    76,    77,    75,    78,
      79,  -138,  -138,  -138,  -138,    80,    81,    84,    85,    86,
      87,  -138,  -138,    88,    82,    92,    89,    90,    93,    94,
      95,   -18,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   109,   107,   108,   110,    96,   111,   112,   113,   114,
      97,   115,   119,   120,   118,   121,   122,   123,   125,  -138,
     126,   127,   128,   129,  -138,   130,   131,   124,   133,   135,
     134,   136,   137,   138,   139,   140,   141,   142,   146,  -138,
     144,   145,    -1,    -1,   149,   150,   151,   152,   153,   154,
     147,   156,   157,   155,   158,   159,  -138,  -138,  -138,   160,
     161,   164,   167,   168,   169,   170,   166,  -138,   172,   173,
     163,   177,   178,  -138,  -138,   179,   180,   181,   182,   183,
    -138,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   197,   199,   200,   201,   203,   204,   205,
     206,  -138,   207,   208,   209,   210,   202,   211,   212,   213,
     214,   215,   216,   218,   219,  -138,   220,   222,  -138,   224,
    -138,   223,   225,   226,   227,   228,   217,   232,   233,   234,
     235,   236,  -138,   229,   230,   237,   238,   239,  -138,  -138,
     243,   244,   245,   231,   246,   247,  -138,   248,   251,   249,
     250,  -138,   252,   253,   254,   255,  -138
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
       0,     0,     0,     0,     0,    32,    33,    34,    43,    44,
      35,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    51,     0,     0,     0,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,    29,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    47,    49,    48,     0,
       0,     0,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,    55,    50,     0,     0,     0,     0,     0,
      31,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    45,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    39,     0,     0,    18,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    28,     0,     0,     0,     0,     0,    40,    41,
       0,     0,     0,     0,     0,     0,    42,     0,     0,     0,
       0,    15,     0,     0,     0,     0,    19
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
      -138,  -138,  -138,  -138,     1,  -138,  -138,  -138,  -138,  -138,
    -138,   256,  -138,  -138,  -138,   162,  -138,    -5,   240,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -137,  -138,   -81,  -138,
    -138
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,     2,     8,    13,    19,    20,    21,    22,    23,    24,
      35,    36,    37,    38,    39,    47,    74,   111,   112,   113,
     196,   197,   198,    77,    78,    79,   199,   114,   115,    80,
      81
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -1;
  const unsigned short int
  Parser::yytable_[] =
  {
        69,    70,     5,     6,     7,     1,    69,    70,   109,   110,
      71,    72,    73,    57,    32,    33,   193,     3,    58,   194,
     195,    29,    30,    41,    42,    31,    10,    11,    12,    16,
      17,    18,     4,    32,    33,    34,    55,    56,   106,   107,
       9,    43,    14,    46,    15,    26,   200,    25,    27,    40,
     145,    28,    45,    75,    50,    44,    51,    52,    53,    64,
      59,    60,    54,    61,    62,    68,     0,    83,    65,     0,
       0,    63,    66,    67,    82,    84,    85,    86,    87,    88,
      90,     0,    89,    92,     0,   101,    91,    93,   104,    94,
      97,    95,    96,    98,    99,   100,   102,   105,   108,   116,
     103,   117,   118,   119,   120,     0,   123,     0,   121,     0,
     124,   125,   128,   122,   126,   127,   138,   131,   132,   129,
     130,   133,   134,   135,   136,   137,   139,   142,   143,   144,
     140,   141,   146,   147,   148,   149,   150,   151,   159,   164,
     154,   152,   153,   155,   156,   157,     0,   158,   160,   161,
     162,   163,   165,   166,   167,   168,   170,   171,   169,   172,
     173,   174,   175,   176,   177,     0,   179,   180,   178,   181,
       0,   182,     0,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   201,   202,   203,   204,   205,   206,   207,
     208,   209,     0,     0,     0,     0,   210,   223,    48,   211,
     212,   215,   213,   214,   216,   217,   218,   219,   220,   221,
     222,   224,   225,   226,   227,   228,   229,   230,   231,   232,
       0,     0,     0,   233,   234,   235,   236,   237,   238,   239,
     240,   243,   242,   244,   245,   246,   241,   247,   248,   249,
     250,   251,     0,     0,   255,   252,   253,   254,   256,   257,
     261,   259,   262,   263,   264,   258,   265,   260,   266,   272,
     267,     0,   268,   269,   270,   271,   273,   274,   275,   276,
     277,   278,   279,   286,   280,   281,   282,   283,   284,   285,
       0,     0,   289,   287,   288,   290,   293,   292,   295,     0,
     294,   291,     0,    49,     0,     0,     0,   296,    76
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
        18,    19,    10,    11,    12,     3,    18,    19,    26,    27,
      22,    23,    24,    36,     7,     8,    17,    36,    41,    20,
      21,    20,    21,    32,    33,    24,    13,    14,    15,     4,
       5,     6,     0,     7,     8,     9,    30,    31,    28,    29,
      37,    41,    38,    16,    39,    36,   183,    39,    36,    34,
     131,    40,    36,    58,    37,    41,    38,    38,    34,    37,
      40,    40,    34,    34,    41,    25,    -1,    34,    37,    -1,
      -1,    41,    38,    38,    38,    34,    34,    34,    41,    41,
      41,    -1,    38,    36,    -1,    35,    41,    41,    34,    41,
      37,    42,    41,    37,    37,    37,    35,    34,    34,    34,
      41,    34,    34,    34,    34,    -1,    34,    -1,    42,    -1,
      37,    37,    37,    42,    38,    38,    34,    37,    37,    41,
      41,    37,    37,    37,    37,    37,    34,    34,    34,    34,
      41,    41,    34,    34,    34,    34,    34,    34,    42,    42,
      34,    37,    37,    34,    37,    37,    -1,    37,    37,    37,
      37,    37,    37,    34,    34,    37,    34,    34,    37,    34,
      34,    34,    34,    34,    34,    -1,    42,    34,    37,    34,
      -1,    37,    -1,    37,    37,    37,    37,    37,    37,    37,
      34,    37,    37,    34,    34,    34,    34,    34,    34,    42,
      34,    34,    -1,    -1,    -1,    -1,    41,    34,    36,    41,
      41,    37,    42,    42,    37,    37,    37,    37,    42,    37,
      37,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      -1,    -1,    -1,    37,    37,    37,    37,    37,    37,    37,
      37,    34,    37,    34,    34,    34,    42,    34,    34,    34,
      34,    34,    -1,    -1,    42,    37,    37,    37,    37,    37,
      34,    37,    34,    34,    34,    42,    34,    42,    34,    42,
      37,    -1,    37,    37,    37,    37,    34,    34,    34,    34,
      34,    42,    42,    42,    37,    37,    37,    34,    34,    34,
      -1,    -1,    34,    37,    37,    34,    34,    37,    34,    -1,
      37,    42,    -1,    37,    -1,    -1,    -1,    42,    58
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     3,    44,    36,     0,    10,    11,    12,    45,    37,
      13,    14,    15,    46,    38,    39,     4,     5,     6,    47,
      48,    49,    50,    51,    52,    39,    36,    36,    40,    47,
      47,    47,     7,     8,     9,    53,    54,    55,    56,    57,
      34,    32,    33,    41,    41,    36,    16,    58,    58,    54,
      37,    38,    38,    34,    34,    30,    31,    36,    41,    40,
      40,    34,    41,    41,    37,    37,    38,    38,    25,    18,
      19,    22,    23,    24,    59,    60,    61,    66,    67,    68,
      72,    73,    38,    34,    34,    34,    34,    41,    41,    38,
      41,    41,    36,    41,    41,    42,    41,    37,    37,    37,
      37,    35,    35,    41,    34,    34,    28,    29,    34,    26,
      27,    60,    61,    62,    70,    71,    34,    34,    34,    34,
      34,    42,    42,    34,    37,    37,    38,    38,    37,    41,
      41,    37,    37,    37,    37,    37,    37,    37,    34,    34,
      41,    41,    34,    34,    34,    71,    34,    34,    34,    34,
      34,    34,    37,    37,    34,    34,    37,    37,    37,    42,
      37,    37,    37,    37,    42,    37,    34,    34,    37,    37,
      34,    34,    34,    34,    34,    34,    34,    34,    37,    42,
      34,    34,    37,    37,    37,    37,    37,    37,    37,    37,
      34,    37,    37,    17,    20,    21,    63,    64,    65,    69,
      69,    34,    34,    34,    34,    34,    34,    42,    34,    34,
      41,    41,    41,    42,    42,    37,    37,    37,    37,    37,
      42,    37,    37,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    37,    37,    37,    37,    37,    37,    37,
      37,    42,    37,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    37,    37,    37,    42,    37,    37,    42,    37,
      42,    34,    34,    34,    34,    34,    34,    37,    37,    37,
      37,    37,    42,    34,    34,    34,    34,    34,    42,    42,
      37,    37,    37,    34,    34,    34,    42,    37,    37,    34,
      34,    42,    37,    34,    37,    34,    42
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
     285,   286,   287,   288,   289,   290,    60,    44,    62,   123,
     125,    40,    41
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    43,    44,    45,    45,    45,    46,    46,    46,    47,
      47,    47,    47,    47,    47,    48,    49,    49,    50,    51,
      52,    52,    53,    53,    54,    54,    55,    55,    56,    57,
      58,    58,    59,    59,    59,    59,    59,    60,    61,    62,
      63,    64,    65,    66,    66,    67,    68,    69,    69,    69,
      70,    71,    71,    71,    71,    72,    73
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     9,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     1,     1,    27,     1,     1,    19,    31,
       5,     5,     7,     7,     2,     1,     1,     1,    20,    10,
       4,    13,     1,     1,     1,     1,     1,     8,    10,    12,
      10,    10,    12,     1,     1,    13,    15,     1,     1,     1,
       8,     1,     1,     1,     1,    10,     6
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
  "WARD", "DIELECTRIC", "CONDUCTOR", "SMOOTH", "FLAT", "AREATYPE",
  "POINTTYPE", "FLOAT", "FILEPATH", "'<'", "','", "'>'", "'{'", "'}'",
  "'('", "')'", "$accept", "scene_file", "accelerator", "tracer",
  "contents", "camera", "light", "pointlight", "arealight", "shape",
  "objfile", "primitive_list", "primitive", "triangle", "sphere",
  "material", "bsdf", "lambert", "phong", "ward", "blinn", "beckmann",
  "aniso", "specular", "specular_dielectric", "specular_conductor",
  "microfacetDistrib", "microfacet", "bxdf", "substrate", "pair", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        44,     0,    -1,     3,    36,    45,    37,    46,    38,    39,
      47,    40,    -1,    10,    -1,    11,    -1,    12,    -1,    13,
      -1,    14,    -1,    15,    -1,    48,    47,    -1,    49,    47,
      -1,    52,    47,    -1,    52,    -1,    48,    -1,    49,    -1,
       5,    36,    34,    37,    34,    38,    41,    34,    37,    34,
      37,    34,    37,    34,    37,    34,    37,    34,    37,    34,
      37,    34,    37,    34,    37,    34,    42,    -1,    50,    -1,
      51,    -1,     6,    36,    33,    38,    41,    34,    37,    34,
      37,    34,    37,    34,    37,    34,    37,    34,    37,    34,
      42,    -1,     6,    36,    32,    38,    41,    34,    37,    34,
      37,    34,    37,    34,    37,    34,    37,    34,    37,    34,
      37,    34,    37,    34,    37,    34,    37,    34,    37,    34,
      37,    34,    42,    -1,     4,    39,    54,    58,    40,    -1,
       4,    39,    53,    58,    40,    -1,     9,    36,    30,    38,
      41,    35,    42,    -1,     9,    36,    31,    38,    41,    35,
      42,    -1,    55,    54,    -1,    55,    -1,    56,    -1,    57,
      -1,     7,    41,    34,    37,    34,    37,    34,    37,    34,
      37,    34,    37,    34,    37,    34,    37,    34,    37,    34,
      42,    -1,     8,    41,    34,    37,    34,    37,    34,    37,
      34,    42,    -1,    16,    41,    59,    42,    -1,    16,    36,
      25,    38,    41,    34,    37,    34,    37,    34,    37,    34,
      42,    -1,    60,    -1,    61,    -1,    66,    -1,    72,    -1,
      73,    -1,    19,    41,    34,    37,    34,    37,    34,    42,
      -1,    18,    41,    34,    37,    34,    37,    34,    37,    34,
      42,    -1,    27,    41,    34,    37,    34,    37,    34,    37,
      34,    37,    34,    42,    -1,    17,    41,    34,    37,    34,
      37,    34,    37,    34,    42,    -1,    20,    41,    34,    37,
      34,    37,    34,    37,    34,    42,    -1,    21,    41,    34,
      37,    34,    37,    34,    37,    34,    37,    34,    42,    -1,
      67,    -1,    68,    -1,    22,    36,    28,    38,    41,    34,
      37,    34,    37,    34,    37,    34,    42,    -1,    22,    36,
      29,    38,    41,    34,    37,    34,    37,    34,    37,    34,
      37,    34,    42,    -1,    63,    -1,    65,    -1,    64,    -1,
      26,    41,    34,    37,    34,    37,    69,    42,    -1,    70,
      -1,    60,    -1,    61,    -1,    62,    -1,    23,    41,    34,
      37,    34,    37,    34,    37,    69,    42,    -1,    24,    41,
      71,    37,    71,    42,    -1
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
     255,   266,   277,   290,   292,   294,   308,   324,   326,   328,
     330,   339,   341,   343,   345,   347,   358
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   107,   107,   115,   116,   117,   121,   122,   123,   127,
     128,   129,   130,   130,   130,   134,   142,   143,   147,   152,
     156,   166,   170,   173,   178,   179,   183,   184,   188,   193,
     198,   199,   204,   205,   206,   207,   208,   212,   217,   224,
     229,   234,   239,   243,   244,   248,   258,   267,   268,   269,
     273,   280,   281,   282,   283,   287,   296
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
      41,    42,     2,     2,    37,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      36,     2,    38,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    39,     2,    40,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 298;
  const int Parser::yynnts_ = 31;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 4;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 43;

  const unsigned int Parser::yyuser_token_number_max_ = 290;
  const Parser::token_number_type Parser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 4 "../src/scene/sceneparser.y"
} // Bison

/* Line 1054 of lalr1.cc  */
#line 1431 "sceneparser.tab.c"


/* Line 1056 of lalr1.cc  */
#line 304 "../src/scene/sceneparser.y"


void Bison::Parser::error(const Bison::Parser::location_type& loc, const std::string& msg){
    cerr << "Parse error at: " << loc << ": " << msg << endl;
}

#include "scanner.hpp"
static int yylex(Bison::Parser::semantic_type * yylval,
                 Bison::Parser::location_type * yylloc,
                 scanner &scan) {
    return scan.yylex(yylval, yylloc);
}

