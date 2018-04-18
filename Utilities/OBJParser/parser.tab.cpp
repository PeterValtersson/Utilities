// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.
// //                    "%code top" blocks.
#line 32 "parser.y" // lalr1.cc:391

    #include <iostream>
    #include "scanner.h"
    #include "parser.tab.h"
    #include "ObjParser.h"
    #include "location.hh"
    
    // yylex() arguments are defined in parser.y
    static ObjParser::Parser::symbol_type yylex(ObjParser::Scanner &scanner, ObjParser::Interpreter &driver) {
        return scanner.get_next_token();
    }
    
    // you can accomplish the same thing by inlining the code using preprocessor
    // x and y are same as in above static function
    // #define yylex(x, y) scanner.get_next_token()
    
    using namespace ObjParser;

#line 53 "parser.tab.cpp" // lalr1.cc:391


// First part of user declarations.

#line 58 "parser.tab.cpp" // lalr1.cc:398

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#include "parser.tab.h"

// User implementation prologue.

#line 72 "parser.tab.cpp" // lalr1.cc:406


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 9 "parser.y" // lalr1.cc:473
namespace  ObjParser  {
#line 158 "parser.tab.cpp" // lalr1.cc:473

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
   Parser ::yytnamerr_ (const char *yystr)
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
              // Fall through.
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


  /// Build a parser object.
   Parser :: Parser  (ObjParser::Scanner &scanner_yyarg, ObjParser::Interpreter &driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

   Parser ::~ Parser  ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
   Parser ::by_state::by_state ()
    : state (empty)
  {}

  inline
   Parser ::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
   Parser ::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
   Parser ::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
   Parser ::symbol_number_type
   Parser ::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
   Parser ::stack_symbol_type::stack_symbol_type ()
  {}


  inline
   Parser ::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 15: // BOOLEAN
        value.move< bool > (that.value);
        break;

      case 3: // REAL
      case 20: // real
      case 22: // optreal
      case 23: // optreal2
        value.move< float > (that.value);
        break;

      case 5: // POSITION
      case 6: // TEXCOORD
      case 7: // NORMAL
      case 8: // POINT
      case 9: // FACE
      case 10: // OBJECT
      case 11: // S
      case 12: // MTLLIB
      case 13: // USEMTL
      case 14: // NAME
      case 16: // SEP
      case 21: // optname
        value.move< std::string > (that.value);
        break;

      case 24: // createface
        value.move< std::vector<std::vector<uint32_t>> > (that.value);
        break;

      case 25: // indices
        value.move< std::vector<uint32_t> > (that.value);
        break;

      case 4: // INTEGER
        value.move< uint32_t > (that.value);
        break;

      case 18: // line
      case 19: // statement
        value.move< void* > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty;
  }

  inline
   Parser ::stack_symbol_type&
   Parser ::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 15: // BOOLEAN
        value.copy< bool > (that.value);
        break;

      case 3: // REAL
      case 20: // real
      case 22: // optreal
      case 23: // optreal2
        value.copy< float > (that.value);
        break;

      case 5: // POSITION
      case 6: // TEXCOORD
      case 7: // NORMAL
      case 8: // POINT
      case 9: // FACE
      case 10: // OBJECT
      case 11: // S
      case 12: // MTLLIB
      case 13: // USEMTL
      case 14: // NAME
      case 16: // SEP
      case 21: // optname
        value.copy< std::string > (that.value);
        break;

      case 24: // createface
        value.copy< std::vector<std::vector<uint32_t>> > (that.value);
        break;

      case 25: // indices
        value.copy< std::vector<uint32_t> > (that.value);
        break;

      case 4: // INTEGER
        value.copy< uint32_t > (that.value);
        break;

      case 18: // line
      case 19: // statement
        value.copy< void* > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
   Parser ::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
   Parser ::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
   Parser ::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
   Parser ::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
   Parser ::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
   Parser ::debug_stream () const
  {
    return *yycdebug_;
  }

  void
   Parser ::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


   Parser ::debug_level_type
   Parser ::debug_level () const
  {
    return yydebug_;
  }

  void
   Parser ::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline  Parser ::state_type
   Parser ::yy_lr_goto_state_ (state_type yystate, int yylhs)
  {
    int yyr = yypgoto_[yylhs - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yylhs - yyntokens_];
  }

  inline bool
   Parser ::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
   Parser ::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
   Parser ::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
    int yyn;
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// $$ and @$.
    stack_symbol_type yylhs;

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULL, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (scanner, driver));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
    /* Variants are always initialized to an empty instance of the
       correct type. The default $$=$1 action is NOT applied when using
       variants.  */
      switch (yyr1_[yyn])
    {
      case 15: // BOOLEAN
        yylhs.value.build< bool > ();
        break;

      case 3: // REAL
      case 20: // real
      case 22: // optreal
      case 23: // optreal2
        yylhs.value.build< float > ();
        break;

      case 5: // POSITION
      case 6: // TEXCOORD
      case 7: // NORMAL
      case 8: // POINT
      case 9: // FACE
      case 10: // OBJECT
      case 11: // S
      case 12: // MTLLIB
      case 13: // USEMTL
      case 14: // NAME
      case 16: // SEP
      case 21: // optname
        yylhs.value.build< std::string > ();
        break;

      case 24: // createface
        yylhs.value.build< std::vector<std::vector<uint32_t>> > ();
        break;

      case 25: // indices
        yylhs.value.build< std::vector<uint32_t> > ();
        break;

      case 4: // INTEGER
        yylhs.value.build< uint32_t > ();
        break;

      case 18: // line
      case 19: // statement
        yylhs.value.build< void* > ();
        break;

      default:
        break;
    }


    // Compute the default @$.
    {
      slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
      YYLLOC_DEFAULT (yylhs.location, slice, yylen);
    }

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    try
      {
        switch (yyn)
          {
  case 2:
#line 89 "parser.y" // lalr1.cc:846
    { driver.increaseRow(); }
#line 656 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 3:
#line 90 "parser.y" // lalr1.cc:846
    { driver.increaseRow(); }
#line 662 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 4:
#line 93 "parser.y" // lalr1.cc:846
    { driver.AddSubMesh(yystack_[0].value.as< std::string > ());}
#line 668 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 5:
#line 94 "parser.y" // lalr1.cc:846
    { }
#line 674 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 6:
#line 95 "parser.y" // lalr1.cc:846
    { driver.AddPosition(ArfData::Position(yystack_[3].value.as< float > (),yystack_[2].value.as< float > (),yystack_[1].value.as< float > ())); }
#line 680 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 7:
#line 96 "parser.y" // lalr1.cc:846
    { driver.AddTexCoord(ArfData::TexCoord(yystack_[2].value.as< float > (),yystack_[1].value.as< float > ()));}
#line 686 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 8:
#line 97 "parser.y" // lalr1.cc:846
    { driver.AddNormal(ArfData::Normal(yystack_[2].value.as< float > (),yystack_[1].value.as< float > (),yystack_[0].value.as< float > ())); }
#line 692 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 9:
#line 98 "parser.y" // lalr1.cc:846
    { }
#line 698 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 10:
#line 99 "parser.y" // lalr1.cc:846
    { }
#line 704 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 11:
#line 100 "parser.y" // lalr1.cc:846
    { }
#line 710 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 12:
#line 101 "parser.y" // lalr1.cc:846
    { }
#line 716 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 13:
#line 102 "parser.y" // lalr1.cc:846
    { driver.AddFace(ArfData::Face(yystack_[0].value.as< std::vector<std::vector<uint32_t>> > ()));}
#line 722 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 14:
#line 105 "parser.y" // lalr1.cc:846
    { yylhs.value.as< float > () = yystack_[0].value.as< float > (); }
#line 728 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 15:
#line 106 "parser.y" // lalr1.cc:846
    { yylhs.value.as< float > () = static_cast<float>(yystack_[0].value.as< uint32_t > ());}
#line 734 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 16:
#line 109 "parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > ()= yystack_[0].value.as< std::string > ();}
#line 740 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 17:
#line 110 "parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = "default";}
#line 746 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 18:
#line 113 "parser.y" // lalr1.cc:846
    { yylhs.value.as< float > () = yystack_[0].value.as< float > (); }
#line 752 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 19:
#line 114 "parser.y" // lalr1.cc:846
    { yylhs.value.as< float > () = 1.0f; }
#line 758 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 20:
#line 117 "parser.y" // lalr1.cc:846
    { yylhs.value.as< float > () = yystack_[0].value.as< float > ();}
#line 764 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 21:
#line 118 "parser.y" // lalr1.cc:846
    { yylhs.value.as< float > () = 0.0f;}
#line 770 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 22:
#line 121 "parser.y" // lalr1.cc:846
    { yylhs.value.as< std::vector<std::vector<uint32_t>> > ().push_back(yystack_[0].value.as< std::vector<uint32_t> > ()); }
#line 776 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 23:
#line 122 "parser.y" // lalr1.cc:846
    { yylhs.value.as< std::vector<std::vector<uint32_t>> > () = yystack_[1].value.as< std::vector<std::vector<uint32_t>> > (); yylhs.value.as< std::vector<std::vector<uint32_t>> > ().push_back(yystack_[0].value.as< std::vector<uint32_t> > ());}
#line 782 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 24:
#line 125 "parser.y" // lalr1.cc:846
    { yylhs.value.as< std::vector<uint32_t> > ().push_back(yystack_[0].value.as< uint32_t > () - 1); }
#line 788 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 25:
#line 126 "parser.y" // lalr1.cc:846
    { yylhs.value.as< std::vector<uint32_t> > () = yystack_[3].value.as< std::vector<uint32_t> > (); yylhs.value.as< std::vector<uint32_t> > ().push_back(UINT32_MAX); yylhs.value.as< std::vector<uint32_t> > ().push_back(yystack_[0].value.as< uint32_t > () - 1);}
#line 794 "parser.tab.cpp" // lalr1.cc:846
    break;

  case 26:
#line 127 "parser.y" // lalr1.cc:846
    { yylhs.value.as< std::vector<uint32_t> > () = yystack_[2].value.as< std::vector<uint32_t> > (); yylhs.value.as< std::vector<uint32_t> > ().push_back(yystack_[0].value.as< uint32_t > () - 1);}
#line 800 "parser.tab.cpp" // lalr1.cc:846
    break;


#line 804 "parser.tab.cpp" // lalr1.cc:846
          default:
            break;
          }
      }
    catch (const syntax_error& yyexc)
      {
        error (yyexc);
        YYERROR;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yylhs);
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    // Shift the result of the reduction.
    yypush_ (YY_NULL, yylhs);
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
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
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* $$ was initialized before running the user action.  */
    YY_SYMBOL_PRINT ("Error: discarding", yylhs);
    yylhs.~stack_symbol_type();
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULL, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULL, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
   Parser ::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
   Parser ::yysyntax_error_ (state_type yystate, symbol_number_type yytoken) const
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULL;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char  Parser ::yypact_ninf_ = -8;

  const signed char  Parser ::yytable_ninf_ = -1;

  const signed char
   Parser ::yypact_[] =
  {
      38,     1,     1,     1,     1,     2,    -7,    27,    -6,    -5,
      12,    -8,    -8,    -8,     1,     1,     1,     1,    -8,     2,
      11,    -8,    -8,    -8,    -8,    -4,    -8,    -8,    -8,     1,
       1,     1,     1,    11,    22,    -8,     1,    -8,    -8,    -8,
      -8,    -8,    -8,     7,    -8,    -8
  };

  const unsigned char
   Parser ::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,    17,     0,     0,     0,
       0,     2,    14,    15,     0,     0,     0,     0,    24,    13,
      22,    16,     4,    12,    11,     0,    10,     1,     3,     0,
      21,     0,    19,    23,     0,     5,    19,    20,     7,     8,
      18,     9,    26,     0,     6,    25
  };

  const signed char
   Parser ::yypgoto_[] =
  {
      -8,    -8,    23,    -1,    -8,    -2,    -8,    -8,    13
  };

  const signed char
   Parser ::yydefgoto_[] =
  {
      -1,    10,    11,    40,    22,    41,    38,    19,    20
  };

  const unsigned char
   Parser ::yytable_[] =
  {
      14,    15,    16,    17,    12,    13,    18,    21,    25,    26,
      35,    45,    27,    29,    30,    31,    32,     1,     2,     3,
       4,     5,     6,     7,     8,     9,    42,    34,    36,    37,
      39,    23,    33,    28,    44,     0,     0,     0,    43,     0,
       0,     0,    24,     1,     2,     3,     4,     5,     6,     7,
       8,     9
  };

  const signed char
   Parser ::yycheck_[] =
  {
       1,     2,     3,     4,     3,     4,     4,    14,    14,    14,
      14,     4,     0,    14,    15,    16,    17,     5,     6,     7,
       8,     9,    10,    11,    12,    13,     4,    16,    29,    30,
      31,     4,    19,    10,    36,    -1,    -1,    -1,    16,    -1,
      -1,    -1,    15,     5,     6,     7,     8,     9,    10,    11,
      12,    13
  };

  const unsigned char
   Parser ::yystos_[] =
  {
       0,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      18,    19,     3,     4,    20,    20,    20,    20,     4,    24,
      25,    14,    21,     4,    15,    14,    14,     0,    19,    20,
      20,    20,    20,    25,    16,    14,    20,    20,    23,    20,
      20,    22,     4,    16,    22,     4
  };

  const unsigned char
   Parser ::yyr1_[] =
  {
       0,    17,    18,    18,    19,    19,    19,    19,    19,    19,
      19,    19,    19,    19,    20,    20,    21,    21,    22,    22,
      23,    23,    24,    24,    25,    25,    25
  };

  const unsigned char
   Parser ::yyr2_[] =
  {
       0,     2,     1,     2,     2,     3,     5,     4,     4,     4,
       2,     2,     2,     2,     1,     1,     1,     0,     1,     0,
       1,     0,     1,     2,     1,     4,     3
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const  Parser ::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "REAL", "INTEGER", "POSITION",
  "TEXCOORD", "NORMAL", "POINT", "FACE", "OBJECT", "S", "MTLLIB", "USEMTL",
  "NAME", "BOOLEAN", "SEP", "$accept", "line", "statement", "real",
  "optname", "optreal", "optreal2", "createface", "indices", YY_NULL
  };

#if YYDEBUG
  const unsigned char
   Parser ::yyrline_[] =
  {
       0,    89,    89,    90,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   105,   106,   109,   110,   113,   114,
     117,   118,   121,   122,   125,   126,   127
  };

  // Print the state stack on the debug stream.
  void
   Parser ::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
   Parser ::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 9 "parser.y" // lalr1.cc:1156
} //  ObjParser 
#line 1199 "parser.tab.cpp" // lalr1.cc:1156
#line 130 "parser.y" // lalr1.cc:1157


// Bison expects us to provide implementation - otherwise linker complains
void ObjParser::Parser::error(const location &loc , const std::string &message) {
        
        // Location should be initialized inside scanner action, but is not in this example.
        // Let's grab location directly from driver class.
	// cout << "Error: " << message << endl << "Location: " << loc << endl;
	
       driver.PutError(message + "\nRow: " + std::to_string(driver.row() + 1) + "\nUnexpected '" + driver.errorCharacter() + "' at location " + std::to_string(driver.location()) + "\n");
}
