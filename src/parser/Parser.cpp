// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

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

// Take the name prefix into account.
#define yylex   Coollex

// First part of user declarations.
#line 4 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:404
 /*** C/C++ Declarations ***/
#include "driver.h"
#include "Scanner.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <stack>
#include "Nodes.h"
#include "StringContainer.h"
#include "Environment.h"
#include "Types.h"
using namespace Cool;

#line 52 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "Parser.h"

// User implementation prologue.
#line 139 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:412

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lex

    std::vector<CaseNode*> cases;   //for MatchNode, contains all cases of match
    std::vector<Node*> acs, exprs, fts;     //for exprs as actuals, exprs inside block, feautures
    std::vector<FormalNode*>fms; //for FormalsNode, contains formals
    std::vector<FunctionNode*>funcs;      //class functions
    std::vector<ClassVariableNode*>vars;  //class variables
    std::vector<BlockNode*>blocks;        //class blocks

#line 80 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:412


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
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace Cool {
#line 166 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:479

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
  Parser::Parser (class Driver& driver_yyarg, class Environment& env_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg),
      env (env_yyarg)
  {}

  Parser::~Parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  Parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::clear ()
  {
    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  Parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
  Parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  Parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  Parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  Parser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  Parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  Parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  Parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  Parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  Parser::symbol_number_type
  Parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  Parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  Parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  Parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  Parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  Parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
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
#endif // YYDEBUG

  inline Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 45 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:741
{
    // initialize the initial location object
    yyla.location.begin.filename = yyla.location.end.filename = driver.streamname;
//@$.initialize(driver.streamname,0,0);
}

#line 547 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:741

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

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
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
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
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

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
#line 157 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.fsNode) = env.mk<FormalsNode>(yystack_[1].location);}
#line 657 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 158 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.fsNode) = env.mk<FormalsNode>(yystack_[2].location,fms); fms.clear(); }
#line 663 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 161 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {fms.push_back(env.mk<FormalNode>(env.mk<ObjectNode>(*(yystack_[2].value.strVal),yystack_[2].location),(yystack_[0].value.typeNode),yystack_[3].location));}
#line 669 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 162 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {fms.push_back(env.mk<FormalNode>(env.mk<ObjectNode>(*(yystack_[2].value.strVal),yystack_[2].location),(yystack_[0].value.typeNode),yystack_[3].location));}
#line 675 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 164 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {yyerrok;}
#line 681 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 165 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {}
#line 687 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 171 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.clNode) = env.mk<ClassNode>(yystack_[3].location,(yystack_[2].value.typeNode),(yystack_[1].value.fsNode),env.mk<ExtendNode>(yystack_[3].location,env.mk<TypeNode>(Types::ANY,yystack_[3].location), env.mk<ActualsNode>(yystack_[3].location)),(yystack_[0].value.clbNode));}
#line 693 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 172 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.clNode) = env.mk<ClassNode>(yystack_[6].location,(yystack_[5].value.typeNode),(yystack_[4].value.fsNode),env.mk<ExtendNode>(yystack_[3].location,(yystack_[2].value.typeNode),(yystack_[1].value.acNode)),(yystack_[0].value.clbNode));}
#line 699 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 173 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.clNode) = env.mk<ClassNode>(yystack_[5].location,(yystack_[4].value.typeNode),(yystack_[3].value.fsNode),env.mk<ExtendNode>(env.mk<NativeNode>(yystack_[1].location),yystack_[2].location),(yystack_[0].value.clbNode));}
#line 705 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 175 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {error(yystack_[3].location,"class name must begin with a capital letter.");yyerrok;}
#line 711 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 176 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {error(yystack_[6].location,"class name must begin with a capital letter.");yyerrok;}
#line 717 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 177 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {error(yystack_[4].location,"class name must begin with a capital letter.");yyerrok;}
#line 723 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 178 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {error(yystack_[0].location,"forgot constructor arguments for class \'" + *(yystack_[1].value.strVal) + "\'");}
#line 729 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 181 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.clbNode) = env.mk<ClassBodyNode>(fts,vars,funcs,blocks,yystack_[2].location);fts.clear();vars.clear();funcs.clear();blocks.clear();}
#line 735 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 184 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {}
#line 741 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 185 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {FunctionNode * n = env.mk<FunctionNode>(false,*(yystack_[6].value.strVal),(yystack_[5].value.fsNode),(yystack_[3].value.typeNode),(yystack_[1].value.node),yystack_[7].location); fts.push_back(n); funcs.push_back(n);}
#line 747 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 186 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {FunctionNode * n = env.mk<FunctionNode>(true,*(yystack_[6].value.strVal),(yystack_[5].value.fsNode),(yystack_[3].value.typeNode),(yystack_[1].value.node),yystack_[8].location); fts.push_back(n); funcs.push_back(n);}
#line 753 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 187 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {ClassVariableNode * n = env.mk<ClassVariableNode>(env.mk<IdNode>(*(yystack_[3].value.strVal),yystack_[3].location),env.mk<NativeNode>(yystack_[1].location),yystack_[4].location); fts.push_back(n); vars.push_back(n);}
#line 759 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 188 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {ClassVariableNode * n = env.mk<ClassVariableNode>(env.mk<IdNode>(*(yystack_[5].value.strVal),yystack_[5].location),(yystack_[3].value.typeNode),(yystack_[1].value.node),yystack_[6].location); fts.push_back(n); vars.push_back(n);}
#line 765 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 189 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {fts.push_back((yystack_[2].value.blNode)); blocks.push_back((yystack_[2].value.blNode));}
#line 771 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 190 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {}
#line 777 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 191 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {yyerrok;}
#line 783 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 193 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    { (yylhs.value.node) = (yystack_[0].value.node);}
#line 789 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 194 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    { (yylhs.value.node) = env.mk<NativeNode>(yystack_[0].location);}
#line 795 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 198 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.fsNode) = env.mk<FormalsNode>(yystack_[2].location,fms); fms.clear();}
#line 801 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 200 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {}
#line 807 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 201 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {}
#line 813 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 203 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {fms.push_back(env.mk<FormalNode>((yystack_[2].value.objNode),(yystack_[0].value.typeNode),yystack_[2].location));}
#line 819 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 204 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {fms.push_back(env.mk<FormalNode>((yystack_[2].value.objNode),(yystack_[0].value.typeNode),yystack_[2].location));}
#line 825 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 205 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {yyerrok;}
#line 831 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 206 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {}
#line 837 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 209 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.acNode) = env.mk<ActualsNode>(yystack_[2].location,acs); acs.clear();}
#line 843 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 210 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.acNode) = env.mk<ActualsNode>(yystack_[1].location);}
#line 849 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 212 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {acs.insert(acs.begin(),(yystack_[2].value.node));}
#line 855 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 213 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {acs.push_back((yystack_[0].value.node));}
#line 861 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 214 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {}
#line 867 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 215 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {yyerrok;}
#line 873 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 218 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.blNode) = env.mk<BlockNode>();}
#line 879 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 219 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {exprs.insert(exprs.begin(),(yystack_[0].value.node)); (yylhs.value.blNode) = env.mk<BlockNode>(exprs,yystack_[0].location); exprs.clear();}
#line 885 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 220 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.blNode) = env.mk<BlockNode>(exprs,yystack_[0].location); exprs.clear();}
#line 891 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 222 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    { exprs.insert(exprs.begin(),(yystack_[2].value.node)); exprs.push_back((yystack_[0].value.node));}
#line 897 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 223 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    { exprs.insert(exprs.begin(),(yystack_[2].value.node));}
#line 903 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 224 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    { (yylhs.value.node) = env.mk<VariableDefinitionNode>(yystack_[7].location,env.mk<IdNode>(*(yystack_[6].value.strVal),yystack_[6].location),(yystack_[4].value.typeNode),(yystack_[2].value.node)); exprs.insert(exprs.begin(),(yylhs.value.node)); exprs.push_back((yystack_[0].value.node));}
#line 909 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 225 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    { (yylhs.value.node) = env.mk<VariableDefinitionNode>(yystack_[7].location,env.mk<IdNode>(*(yystack_[6].value.strVal),yystack_[6].location),(yystack_[4].value.typeNode),(yystack_[2].value.node)); exprs.insert(exprs.begin(),(yylhs.value.node));}
#line 915 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 228 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.objNode) = env.mk<ObjectNode>(*(yystack_[0].value.strVal),yystack_[0].location);}
#line 921 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 229 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.typeNode) = env.mk<TypeNode>(*(yystack_[0].value.strVal),yystack_[0].location);}
#line 927 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 232 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<AssignNode>((yystack_[2].value.objNode), (yystack_[0].value.node),yystack_[2].location);}
#line 933 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 233 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<NegateNode>(yystack_[1].location,(yystack_[0].value.node));}
#line 939 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 234 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<UnaryMinusNode>(yystack_[1].location,(yystack_[0].value.node));}
#line 945 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 235 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<IfNode>(yystack_[6].location,(yystack_[4].value.node),(yystack_[2].value.node),(yystack_[0].value.node));}
#line 951 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 236 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<WhileNode>(yystack_[4].location,yystack_[0].location.end,(yystack_[2].value.node),(yystack_[0].value.node));}
#line 957 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 237 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<SuperNode>(yystack_[3].location,env.mk<IdNode>(*(yystack_[1].value.strVal),yystack_[1].location),(yystack_[0].value.acNode));}
#line 963 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 238 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<FunctionCallNode>(yystack_[1].location,env.mk<IdNode>(*(yystack_[1].value.strVal),yystack_[1].location),(yystack_[0].value.acNode));}
#line 969 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 239 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<NewNode>(yystack_[2].location,(yystack_[1].value.typeNode),(yystack_[0].value.acNode));}
#line 975 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 240 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = (yystack_[1].value.blNode);}
#line 981 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 241 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<ParenthesesExpressionNode>(yystack_[2].location,(yystack_[1].value.node));}
#line 987 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 242 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<ObjectFunctionCallNode>((yystack_[3].value.node),env.mk<IdNode>(*(yystack_[1].value.strVal),yystack_[1].location),(yystack_[0].value.acNode),yystack_[3].location);}
#line 993 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 243 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<MatchNode>(yystack_[2].location,(yystack_[2].value.node),cases);cases.clear();}
#line 999 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 244 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<LtEqNode>(yystack_[2].location,(yystack_[2].value.node),(yystack_[0].value.node));}
#line 1005 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 245 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<LtNode>(yystack_[2].location,(yystack_[2].value.node),(yystack_[0].value.node));}
#line 1011 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 246 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<ObjectFunctionCallNode>((yystack_[2].value.node),env.mk<IdNode>(StringContainer::get("equals"),yystack_[1].location),env.mk<ActualsNode>(yystack_[0].location,std::vector<Node*>{(yystack_[0].value.node)}),yystack_[2].location);}
#line 1017 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 247 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<MultiplyNode>(yystack_[2].location,(yystack_[2].value.node),(yystack_[0].value.node));}
#line 1023 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 248 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<DivideNode>(yystack_[2].location,(yystack_[2].value.node),(yystack_[0].value.node));}
#line 1029 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 249 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<AddNode>(yystack_[2].location,(yystack_[2].value.node),(yystack_[0].value.node));}
#line 1035 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 250 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<SubtractNode>(yystack_[2].location,(yystack_[2].value.node),(yystack_[0].value.node));}
#line 1041 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 251 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<NullNode>(yystack_[0].location);}
#line 1047 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 252 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<UnitNode>(yystack_[1].location);}
#line 1053 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 253 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = (yystack_[0].value.objNode);}
#line 1059 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 254 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<IntNode>(yystack_[0].location,(yystack_[0].value.intVal)); }
#line 1065 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 255 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<StringNode>(yystack_[0].location,*(yystack_[0].value.strVal));}
#line 1071 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 256 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<StringNode>(yystack_[0].location,*(yystack_[0].value.strVal));}
#line 1077 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 257 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<BoolNode>(yystack_[0].location,(yystack_[0].value.boolVal));}
#line 1083 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 258 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {(yylhs.value.node) = env.mk<ThisNode>(yystack_[0].location);}
#line 1089 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 261 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {}
#line 1095 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 263 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {cases.insert(cases.begin(),env.mk<CaseNode>(yystack_[6].location,env.mk<IdNode>(*(yystack_[5].value.strVal),yystack_[5].location),(yystack_[3].value.typeNode),(yystack_[1].value.blNode)));}
#line 1101 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 264 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {cases.insert(cases.begin(),env.mk<CaseNode>(yystack_[4].location,env.mk<IdNode>(Values::_NULL,yystack_[3].location),env.mk<TypeNode>(Types::_NULL,yystack_[3].location),(yystack_[1].value.blNode)));}
#line 1107 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 266 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {cases.insert(cases.begin(),env.mk<CaseNode>(yystack_[5].location,env.mk<IdNode>(*(yystack_[4].value.strVal),yystack_[4].location),(yystack_[2].value.typeNode),(yystack_[0].value.blNode)));}
#line 1113 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 267 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {cases.insert(cases.begin(),env.mk<CaseNode>(yystack_[3].location,env.mk<IdNode>(Values::_NULL,yystack_[2].location),env.mk<TypeNode>(Types::_NULL,yystack_[2].location),(yystack_[0].value.blNode)));}
#line 1119 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 270 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:859
    {   if(!env.errors.hasCritical()){
                            if(!env.addClass(*(yystack_[0].value.clNode))){
                                error(yystack_[0].location,"Duplicate class definition for \'" + (yystack_[0].value.clNode)->getType() + "\'.");
                            }
                        }
                    }
#line 1130 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
    break;


#line 1134 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:859
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
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
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
    if (!yyla.empty ())
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
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
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
       - The only way there can be no lookahead present (in yyla) is
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
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
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

    char const* yyformat = YY_NULLPTR;
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

    std::string yyres;
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


  const short int Parser::yypact_ninf_ = -153;

  const signed char Parser::yytable_ninf_ = -28;

  const short int
  Parser::yypact_[] =
  {
    -153,    14,  -153,     1,  -153,  -153,     4,    17,    17,    60,
    -153,    11,    -1,     3,  -153,    50,  -153,  -153,    21,    66,
     -12,  -153,    -5,  -153,   202,  -153,    59,    27,    46,    33,
      57,  -153,    12,     4,  -153,    80,     4,    80,   304,   202,
     236,   304,    86,    41,  -153,    85,  -153,    65,    92,  -153,
      80,  -153,  -153,  -153,    94,  -153,    89,   343,  -153,    96,
      74,     2,    41,  -153,    75,  -153,   134,     4,  -153,     4,
      98,   103,  -153,   359,    98,   304,    80,    81,   100,   304,
    -153,   101,   304,   304,   304,   304,   304,   304,   304,   304,
      83,   202,   110,    20,   106,    96,    41,    97,  -153,   107,
     112,  -153,   118,   372,  -153,  -153,  -153,  -153,   386,  -153,
      80,    41,   400,  -153,   458,    61,    61,    98,    98,   180,
     180,    82,    80,  -153,   343,   105,  -153,  -153,   121,   120,
     122,    41,   123,   124,   119,    41,   168,  -153,   168,   304,
    -153,   133,   304,  -153,     5,   130,  -153,    18,    41,   136,
      41,   304,  -153,  -153,  -153,  -153,   413,   304,  -153,   144,
     139,  -153,  -153,   140,  -153,   270,   142,   426,   304,   442,
     202,    41,    41,  -153,   141,   458,   270,  -153,   458,   202,
     105,   149,  -153,  -153,   147,  -153,   343,  -153,   202,  -153,
     105,  -153
  };

  const unsigned char
  Parser::yydefact_[] =
  {
      80,     0,     1,     0,    81,    46,    47,     0,     0,     0,
      14,     0,     0,     0,    22,     0,     7,     2,     0,     0,
       0,    11,     0,     8,    39,    15,     0,     0,     0,     0,
       0,     3,     0,     0,    47,     0,     0,     0,     0,    39,
       0,     0,     0,     0,    67,     0,    74,     0,     0,    73,
      46,    72,    71,    70,     0,    41,    69,    40,    23,     0,
       0,     0,     0,     6,     0,    13,     0,     0,    10,     0,
      50,     0,    68,     0,    49,     0,     0,     0,     0,     0,
      54,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     5,     0,
      37,    34,     0,    36,    12,     9,    56,    57,     0,    55,
       0,     0,     0,    21,    48,    65,    66,    63,    64,    61,
      60,    62,     0,    43,    42,     0,    59,    32,     0,    28,
       0,     0,     0,     0,     0,     0,     0,    33,     0,     0,
      53,     0,     0,    58,     0,     0,    26,     0,     0,     0,
       0,     0,    19,     4,    38,    35,     0,     0,    52,     0,
       0,    75,    31,     0,    30,     0,     0,     0,     0,     0,
      39,     0,     0,    25,     0,    24,     0,    20,    51,     0,
      79,     0,    29,    17,     0,    45,    44,    77,    39,    18,
      78,    76
  };

  const short int
  Parser::yypgoto_[] =
  {
    -153,   155,  -153,  -153,    -3,  -153,     7,    84,  -153,  -153,
     -32,   -39,   -38,   -83,     0,   -20,   -34,  -153,  -152,  -153
  };

  const short int
  Parser::yydefgoto_[] =
  {
      -1,    12,    19,     4,    10,    15,   174,    94,   128,   129,
      67,   102,    54,    55,    56,     8,    57,   126,   145,     1
  };

  const short int
  Parser::yytable_[] =
  {
      35,    71,    37,     7,    70,    69,    73,    74,   123,    21,
      23,     9,    16,    63,     2,     9,     9,    33,    80,   162,
      96,   127,    97,    76,    36,    20,    17,    34,   187,    22,
      65,    11,   103,    68,    34,   -27,   159,     3,   191,     5,
       6,   108,    98,   160,   109,   112,    18,    64,   114,   115,
     116,   117,   118,   119,   120,   121,     5,   124,     5,    30,
      58,    14,    24,    25,   104,    59,   105,    85,    86,    26,
      60,    61,   -16,   -16,    27,    62,   133,    90,   140,   -16,
      34,    31,    28,    32,   -16,    29,    83,    84,    85,    86,
     143,   141,   -16,   130,    66,   -16,   185,   154,    90,   155,
      75,    77,   103,    78,   103,   156,    79,    81,   158,    82,
      93,   149,    95,    99,    90,   153,   106,   167,   111,   110,
     113,   122,   125,   169,   131,   135,   134,   144,   164,   136,
     166,   175,   180,   137,   178,   100,   146,   147,   152,    38,
     148,   150,   175,   161,   151,   186,    39,   163,    40,   101,
     190,   181,   182,   157,   170,    41,   165,   171,   172,   188,
     183,    42,   176,    13,    43,    44,   189,    45,    46,   100,
      48,    49,    50,    38,    51,    52,    53,     0,     0,   132,
      39,     0,    40,   184,    83,    84,    85,    86,     0,    41,
       0,    89,     0,     0,     0,    42,    90,     0,    43,    44,
       0,    45,    46,     0,    48,    49,    50,    38,    51,    52,
      53,     0,     0,     0,    39,     0,    40,     0,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,    42,
       0,     0,    43,    44,     0,    45,    46,    47,    48,    49,
      50,    38,    51,    52,    53,     0,     0,     0,    39,     0,
      40,    72,     0,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,    42,     0,     0,    43,    44,     0,    45,
      46,     0,    48,    49,    50,    38,    51,    52,    53,     0,
       0,     0,    39,     0,    40,     0,     0,     0,     0,     0,
       0,    41,     0,     0,     0,     0,     0,    42,     0,   173,
      43,    44,     0,    45,    46,     0,    48,    49,    50,    38,
      51,    52,    53,     0,     0,     0,    39,     0,    40,     0,
       0,     0,     0,     0,     0,    41,     0,     0,     0,     0,
       0,    42,     0,     0,    43,    44,     0,    45,    46,     0,
      48,    49,    50,     0,    51,    52,    53,    83,    84,    85,
      86,    87,    88,     0,    89,     0,     0,     0,     0,    90,
       0,     0,    91,    83,    84,    85,    86,    87,    88,     0,
      89,    92,     0,     0,   107,    90,    83,    84,    85,    86,
      87,    88,     0,    89,     0,     0,     0,    92,    90,   138,
      83,    84,    85,    86,    87,    88,     0,    89,     0,     0,
      92,   139,    90,     0,    83,    84,    85,    86,    87,    88,
       0,    89,     0,     0,    92,   142,    90,    83,    84,    85,
      86,    87,    88,     0,    89,     0,     0,     0,    92,    90,
      83,    84,    85,    86,    87,    88,     0,    89,   168,     0,
       0,    92,    90,     0,     0,   177,    83,    84,    85,    86,
      87,    88,     0,    89,    92,     0,     0,     0,    90,     0,
       0,   179,    83,    84,    85,    86,    87,    88,     0,    89,
      92,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92
  };

  const short int
  Parser::yycheck_[] =
  {
      20,    39,    22,     3,    38,    37,    40,    41,    91,    12,
      13,    12,     1,     1,     0,    12,    12,    29,    50,     1,
      18,     1,    20,    43,    29,    26,    15,    39,   180,    26,
      33,    14,    66,    36,    39,    15,    31,    23,   190,    38,
      39,    75,    62,    38,    76,    79,    35,    35,    82,    83,
      84,    85,    86,    87,    88,    89,    38,    91,    38,    38,
       1,     1,    12,    13,    67,    38,    69,     6,     7,    19,
      24,    38,    12,    13,    24,    18,    96,    16,   110,    19,
      39,    15,    32,    17,    24,    35,     4,     5,     6,     7,
     122,   111,    32,    93,    14,    35,   179,   136,    16,   138,
      14,    16,   136,    38,   138,   139,    14,    13,   142,    20,
      14,   131,    38,    38,    16,   135,    13,   151,    18,    38,
      19,    38,    12,   157,    18,    18,    29,    22,   148,    17,
     150,   165,   170,    15,   168,     1,    15,    17,    19,     5,
      18,    18,   176,    13,    20,   179,    12,   147,    14,    15,
     188,   171,   172,    20,    10,    21,    20,    18,    18,    10,
      19,    27,    20,     8,    30,    31,    19,    33,    34,     1,
      36,    37,    38,     5,    40,    41,    42,    -1,    -1,    95,
      12,    -1,    14,   176,     4,     5,     6,     7,    -1,    21,
      -1,    11,    -1,    -1,    -1,    27,    16,    -1,    30,    31,
      -1,    33,    34,    -1,    36,    37,    38,     5,    40,    41,
      42,    -1,    -1,    -1,    12,    -1,    14,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    30,    31,    -1,    33,    34,    35,    36,    37,
      38,     5,    40,    41,    42,    -1,    -1,    -1,    12,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    30,    31,    -1,    33,
      34,    -1,    36,    37,    38,     5,    40,    41,    42,    -1,
      -1,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    27,    -1,    29,
      30,    31,    -1,    33,    34,    -1,    36,    37,    38,     5,
      40,    41,    42,    -1,    -1,    -1,    12,    -1,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    30,    31,    -1,    33,    34,    -1,
      36,    37,    38,    -1,    40,    41,    42,     4,     5,     6,
       7,     8,     9,    -1,    11,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    19,     4,     5,     6,     7,     8,     9,    -1,
      11,    28,    -1,    -1,    15,    16,     4,     5,     6,     7,
       8,     9,    -1,    11,    -1,    -1,    -1,    28,    16,    17,
       4,     5,     6,     7,     8,     9,    -1,    11,    -1,    -1,
      28,    15,    16,    -1,     4,     5,     6,     7,     8,     9,
      -1,    11,    -1,    -1,    28,    15,    16,     4,     5,     6,
       7,     8,     9,    -1,    11,    -1,    -1,    -1,    28,    16,
       4,     5,     6,     7,     8,     9,    -1,    11,    25,    -1,
      -1,    28,    16,    -1,    -1,    19,     4,     5,     6,     7,
       8,     9,    -1,    11,    28,    -1,    -1,    -1,    16,    -1,
      -1,    19,     4,     5,     6,     7,     8,     9,    -1,    11,
      28,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    28
  };

  const unsigned char
  Parser::yystos_[] =
  {
       0,    65,     0,    23,    49,    38,    39,    60,    61,    12,
      50,    14,    47,    47,     1,    51,     1,    15,    35,    48,
      26,    50,    26,    50,    12,    13,    19,    24,    32,    35,
      38,    15,    17,    29,    39,    61,    29,    61,     5,    12,
      14,    21,    27,    30,    31,    33,    34,    35,    36,    37,
      38,    40,    41,    42,    58,    59,    60,    62,     1,    38,
      24,    38,    18,     1,    35,    50,    14,    56,    50,    56,
      62,    58,    15,    62,    62,    14,    61,    16,    38,    14,
      56,    13,    20,     4,     5,     6,     7,     8,     9,    11,
      16,    19,    28,    14,    53,    38,    18,    20,    61,    38,
       1,    15,    57,    62,    50,    50,    13,    15,    62,    56,
      38,    18,    62,    19,    62,    62,    62,    62,    62,    62,
      62,    62,    38,    59,    62,    12,    63,     1,    54,    55,
      60,    18,    53,    61,    29,    18,    17,    15,    17,    15,
      56,    61,    15,    56,    22,    64,    15,    17,    18,    61,
      18,    20,    19,    61,    57,    57,    62,    20,    62,    31,
      38,    13,     1,    60,    61,    20,    61,    62,    25,    62,
      10,    18,    18,    29,    52,    62,    20,    19,    62,    19,
      58,    61,    61,    19,    52,    59,    62,    64,    10,    19,
      58,    64
  };

  const unsigned char
  Parser::yyr1_[] =
  {
       0,    46,    47,    47,    48,    48,    48,    48,    49,    49,
      49,    49,    49,    49,    49,    50,    51,    51,    51,    51,
      51,    51,    51,    51,    52,    52,    53,    54,    54,    55,
      55,    55,    55,    56,    56,    57,    57,    57,    57,    58,
      58,    58,    59,    59,    59,    59,    60,    61,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    63,    64,    64,    64,    64,
      65,    65
  };

  const unsigned char
  Parser::yyr2_[] =
  {
       0,     2,     2,     3,     6,     4,     3,     1,     4,     7,
       6,     4,     7,     6,     3,     3,     0,     9,    10,     6,
       8,     5,     1,     3,     1,     1,     3,     0,     1,     5,
       3,     3,     1,     3,     2,     3,     1,     1,     3,     0,
       1,     1,     3,     3,     8,     8,     1,     1,     3,     2,
       2,     7,     5,     4,     2,     3,     3,     3,     4,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     3,     7,     5,     6,     4,
       0,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"end of line\"", "\"+\"",
  "\"-\"", "\"*\"", "\"/\"", "\"<\"", "\"<=\"", "\"=>\"", "\"==\"",
  "\"{\"", "\"}\"", "\"(\"", "\")\"", "\".\"", "\",\"", "\":\"", "\";\"",
  "\"=\"", "\"!\"", "\"case\"", "\"class definition\"", "\"def\"",
  "\"else\"", "\"extend\"", "\"if\"", "\"match\"", "\"native\"", "\"new\"",
  "\"null\"", "\"override\"", "\"super\"", "\"this\"", "\"var\"",
  "\"while\"", "\"Boolean\"", "\"identifier\"", "\"type\"",
  "\"three-quote-string\"", "\"string\"", "\"Int\"", "\"undefined\"",
  "UNARY", "W", "$accept", "vf", "vf2", "cd", "cb", "ft", "ft2", "fm",
  "fm2", "fm3", "ac", "ac2", "bl", "bl2", "id", "type", "ex", "ca", "ca2",
  "start", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  Parser::yyrline_[] =
  {
       0,   157,   157,   158,   161,   162,   164,   165,   171,   172,
     173,   175,   176,   177,   178,   181,   184,   185,   186,   187,
     188,   189,   190,   191,   193,   194,   198,   200,   201,   203,
     204,   205,   206,   209,   210,   212,   213,   214,   215,   218,
     219,   220,   222,   223,   224,   225,   228,   229,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   261,   263,   264,   266,   267,
     269,   270
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
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
  Parser::yy_reduce_print_ (int yyrule)
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

  // Symbol number corresponding to token number t.
  inline
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45
    };
    const unsigned int user_token_number_max_ = 300;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // Cool
#line 1736 "/home/ldejong/engineering/cool-compiler/src/parser/Parser.cpp" // lalr1.cc:1167
#line 280 "/home/ldejong/engineering/cool-compiler/src/parser/parser.yy" // lalr1.cc:1168
 /*** Additional Code ***/
void Cool::Parser::error(const Parser::location_type& l, const std::string& m)
{
    env.errors.registerError(l,m);
}
