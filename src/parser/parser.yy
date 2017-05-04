/* $Id: parser.yy 48 2009-09-05 08:07:10Z tb $ -*- mode: c++ -*- */
/** \file parser.yy Contains the Cool Bison parser source */

%{ /*** C/C++ Declarations ***/
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
%}

/*** yacc/bison Declarations ***/

/* Require bison 2.3 or later */
%require "2.3"

/* add debug output code to generated parser. disable this for release
 * versions. */
%debug

/* start symbol is named "start" */
%start start

/* write out a header file containing the token defines */
/*%defines "Parser.h"*/

/* use newer C++ skeleton file */
%skeleton "lalr1.cc"

/* namespace to enclose parser in */
%name-prefix "Cool"

/* set the parser's class identifier */
%define parser_class_name {Parser}

/* keep track of the current position within the input */
%locations
%initial-action
{
    // initialize the initial location object
    @$.begin.filename = @$.end.filename = driver.streamname;
//@$.initialize(driver.streamname,0,0);
};

/* The driver is passed by reference to the parser and to the scanner. This
 * provides a simple but effective pure interface, not relying on global
 * variables. 
    classes is the map that the parser stores the parsed classes in */
%parse-param {class Driver& driver} {class Environment& env};

/* verbose error messages */
%error-verbose

%union {
    int                 intVal;
    const std::string*  strVal;
    bool                boolVal;
    class Node*         node;
    class ObjectNode*   objNode;
    class TypeNode*     typeNode;
    class FormalsNode*  fsNode;
    class ActualsNode*  acNode;
    class ClassNode*    clNode;
    class ClassBodyNode*clbNode;
    class BlockNode*    blNode;
}

%token			ENDOF	     0	"end of file"
%token			EOL		"end of line"
%token                  PLUS            "+"
%token                  MINUS           "-"
%token                  AST             "*"
%token                  FSLASH          "/"
%token                  LT              "<"
%token                  LTEQ            "<="
%token                  DO              "=>"
%token                  EQ              "=="
%token                  BOPEN           "{"
%token                  BCLOSE          "}"
%token                  POPEN           "("
%token                  PCLOSE          ")"
%token                  PERIOD          "."
%token                  COMMA           ","
%token                  COLON           ":"
%token                  SCOLON          ";"
%token                  ASSGN           "="
%token                  BANG            "!"
%token                  KCASE           "case"
%token                  KCLASS          "class definition"
%token                  KDEF            "def"
%token                  KELSE           "else"
%token                  KEXT            "extend"
%token                  KIF             "if"
%token                  KMATCH          "match"
%token                  KNAT            "native"
%token                  KNEW            "new"
%token                  KNULL           "null"
%token                  KOVER           "override"
%token                  KSUPER          "super"
%token                  KTHIS           "this"
%token                  KVAR            "var"
%token                  KWHILE          "while"
%token <boolVal>        TBOOL           "Boolean"
%token <strVal>         OBJID           "identifier"
%token <strVal>         TYPEID          "type"
%token <strVal>         TCSTR           "three-quote-string"
%token <strVal>         TSTR            "string"
%token <intVal>         TINT            "Int"
%token                  UNDEF           "undefined"


%left ASSGN
%left KELSE
%left KIF KWHILE
%left KMATCH
%left LTEQ LT
%left EQ
%left PLUS MINUS
%left AST FSLASH
%left BANG UNARY
%left PERIOD
%left W

%type <typeNode>    type
%type <objNode>      id
%type <fsNode>      fm vf
%type <acNode>      ac
%type <blNode>      bl
%type <clNode>      cd
%type <clbNode>     cb
%type <node>        ac2 bl2 ca ca2 ex ft ft2

%{
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
%}

%% /*** Grammar Rules ***/

/*var formals*/
vf      : POPEN PCLOSE      {$$ = env.mk<FormalsNode>(@1);}
        | POPEN vf2 PCLOSE  {$$ = env.mk<FormalsNode>(@1,fms); fms.clear(); }


vf2     : vf2 COMMA KVAR OBJID COLON type   {fms.push_back(env.mk<FormalNode>(env.mk<ObjectNode>(*$4,@4),$6,@3));}
        | KVAR OBJID COLON type             {fms.push_back(env.mk<FormalNode>(env.mk<ObjectNode>(*$2,@2),$4,@1));}
        //ERRORS
        | vf2  COMMA error{yyerrok;}
        | error {}
     
        
 

/*class declaration*/
cd      : KCLASS type vf cb                 {$$ = env.mk<ClassNode>(@1,$2,$3,env.mk<ExtendNode>(@1,env.mk<TypeNode>(Types::ANY,@1), env.mk<ActualsNode>(@1)),$4);}
        | KCLASS type vf KEXT type ac cb    {$$ = env.mk<ClassNode>(@1,$2,$3,env.mk<ExtendNode>(@4,$5,$6),$7);}
        | KCLASS type vf KEXT KNAT cb       {$$ = env.mk<ClassNode>(@1,$2,$3,env.mk<ExtendNode>(env.mk<NativeNode>(@5),@4),$6);}
        //ERRORS
        | KCLASS id   vf cb                 {error(@1,"class name must begin with a capital letter.");yyerrok;}
        | KCLASS id vf KEXT type ac cb      {error(@1,"class name must begin with a capital letter.");yyerrok;}
        | KCLASS id vf KEXT KNAT cb         {error(@2,"class name must begin with a capital letter.");yyerrok;}
        | KCLASS TYPEID cb                  {error(@3,"forgot constructor arguments for class \'" + *$2 + "\'");}

/*class body*/
cb      : BOPEN ft BCLOSE                  {$$ = env.mk<ClassBodyNode>(fts,vars,funcs,blocks,@1);fts.clear();vars.clear();funcs.clear();blocks.clear();}

/*feature*/
ft      : /*empty*/{}
        | ft KDEF OBJID fm COLON type ASSGN ft2 SCOLON          {FunctionNode * n = env.mk<FunctionNode>(false,*$3,$4,$6,$8,@2); fts.push_back(n); funcs.push_back(n);}
        | ft KOVER KDEF OBJID fm COLON type ASSGN ft2 SCOLON    {FunctionNode * n = env.mk<FunctionNode>(true,*$4,$5,$7,$9,@2); fts.push_back(n); funcs.push_back(n);}
        | ft KVAR OBJID ASSGN KNAT SCOLON                       {ClassVariableNode * n = env.mk<ClassVariableNode>(env.mk<IdNode>(*$3,@3),env.mk<NativeNode>(@5),@2); fts.push_back(n); vars.push_back(n);}
        | ft KVAR OBJID COLON type ASSGN ex SCOLON              {ClassVariableNode * n = env.mk<ClassVariableNode>(env.mk<IdNode>(*$3,@3),$5,$7,@2); fts.push_back(n); vars.push_back(n);}
        | ft BOPEN bl BCLOSE SCOLON                             {fts.push_back($3); blocks.push_back($3);}
        | error {}
        | ft SCOLON error   {yyerrok;}

ft2     : ex    { $$ = $1;}
        | KNAT  { $$ = env.mk<NativeNode>(@1);} 


/*formals*/
fm      : POPEN fm2 PCLOSE  {$$ = env.mk<FormalsNode>(@1,fms); fms.clear();}

fm2     : /*empty*/ {}
        | fm3   {}

fm3     : fm3 COMMA id COLON type   {fms.push_back(env.mk<FormalNode>($3,$5,@3));}
        | id COLON type             {fms.push_back(env.mk<FormalNode>($1,$3,@1));}
        | fm3 COMMA error           {yyerrok;}
        | error {}

/*actuals*/
ac      : POPEN ac2 PCLOSE  {$$ = env.mk<ActualsNode>(@1,acs); acs.clear();}
        | POPEN PCLOSE      {$$ = env.mk<ActualsNode>(@1);}

ac2     : ex COMMA ac2      {acs.insert(acs.begin(),$1);}  
        | ex                {acs.push_back($1);}
        | error{}
        | error COMMA ac2   {yyerrok;}

/*block*/
bl      : /*empty*/     {$$ = env.mk<BlockNode>();}
        | ex            {exprs.insert(exprs.begin(),$1); $$ = env.mk<BlockNode>(exprs,@1); exprs.clear();}
        | bl2           {$$ = env.mk<BlockNode>(exprs,@1); exprs.clear();}
        
bl2     : ex SCOLON ex                              { exprs.insert(exprs.begin(),$1); exprs.push_back($3);}
        | ex SCOLON bl2                             { exprs.insert(exprs.begin(),$1);}
        | KVAR OBJID COLON type ASSGN ex SCOLON ex  { $$ = env.mk<VariableDefinitionNode>(@1,env.mk<IdNode>(*$2,@2),$4,$6); exprs.insert(exprs.begin(),$$); exprs.push_back($8);}
        | KVAR OBJID COLON type ASSGN ex SCOLON bl2 { $$ = env.mk<VariableDefinitionNode>(@1,env.mk<IdNode>(*$2,@2),$4,$6); exprs.insert(exprs.begin(),$$);}


id   : OBJID {$$ = env.mk<ObjectNode>(*$1,@1);}
type  : TYPEID {$$ = env.mk<TypeNode>(*$1,@1);}

/*expression*/
ex   : id ASSGN ex                          {$$ = env.mk<AssignNode>($1, $3,@1);}
     | BANG ex                              {$$ = env.mk<NegateNode>(@1,$2);}
     | MINUS ex %prec UNARY                 {$$ = env.mk<UnaryMinusNode>(@1,$2);}
     | KIF POPEN ex PCLOSE ex KELSE ex      {$$ = env.mk<IfNode>(@1,$3,$5,$7);}
     | KWHILE POPEN ex PCLOSE ex %prec W    {$$ = env.mk<WhileNode>(@1,@5.end,$3,$5);}
     | KSUPER PERIOD OBJID ac               {$$ = env.mk<SuperNode>(@1,env.mk<IdNode>(*$3,@3),$4);}
     | OBJID ac                             {$$ = env.mk<FunctionCallNode>(@1,env.mk<IdNode>(*$1,@1),$2);} /***function name and parameters***/
     | KNEW type ac                         {$$ = env.mk<NewNode>(@1,$2,$3);}
     | BOPEN bl BCLOSE                      {$$ = $2;}
     | POPEN ex PCLOSE                      {$$ = env.mk<ParenthesesExpressionNode>(@1,$2);}
     | ex PERIOD OBJID ac                   {$$ = env.mk<ObjectFunctionCallNode>($1,env.mk<IdNode>(*$3,@3),$4,@1);}
     | ex KMATCH ca                         {$$ = env.mk<MatchNode>(@1,$1,cases);cases.clear();}
     | ex LTEQ ex                           {$$ = env.mk<LtEqNode>(@1,$1,$3);}
     | ex LT ex                             {$$ = env.mk<LtNode>(@1,$1,$3);}
     | ex EQ ex                             {$$ = env.mk<ObjectFunctionCallNode>($1,env.mk<IdNode>(StringContainer::get("equals"),@2),env.mk<ActualsNode>(@3,std::vector<Node*>{$3}),@1);}
     | ex AST ex                            {$$ = env.mk<MultiplyNode>(@1,$1,$3);}
     | ex FSLASH ex                         {$$ = env.mk<DivideNode>(@1,$1,$3);}
     | ex PLUS ex                           {$$ = env.mk<AddNode>(@1,$1,$3);}
     | ex MINUS ex                          {$$ = env.mk<SubtractNode>(@1,$1,$3);}
     | KNULL                                {$$ = env.mk<NullNode>(@1);}
     | POPEN PCLOSE                         {$$ = env.mk<UnitNode>(@1);}
     | id                                   {$$ = $1;}
     | TINT                                 {$$ = env.mk<IntNode>(@1,$1); }
     | TSTR                                 {$$ = env.mk<StringNode>(@1,*$1);}
     | TCSTR                                {$$ = env.mk<StringNode>(@1,*$1);}
     | TBOOL                                {$$ = env.mk<BoolNode>(@1,$1);}
     | KTHIS                                {$$ = env.mk<ThisNode>(@1);}

/*cases*/
ca      : BOPEN ca2 BCLOSE {}

ca2     : KCASE OBJID COLON type DO bl ca2  {cases.insert(cases.begin(),env.mk<CaseNode>(@1,env.mk<IdNode>(*$2,@2),$4,$6));}
        | KCASE KNULL DO bl ca2             {cases.insert(cases.begin(),env.mk<CaseNode>(@1,env.mk<IdNode>(Values::_NULL,@2),env.mk<TypeNode>(Types::_NULL,@2),$4));}
        
        | KCASE OBJID COLON type DO bl      {cases.insert(cases.begin(),env.mk<CaseNode>(@1,env.mk<IdNode>(*$2,@2),$4,$6));}
        | KCASE KNULL DO bl                 {cases.insert(cases.begin(),env.mk<CaseNode>(@1,env.mk<IdNode>(Values::_NULL,@2),env.mk<TypeNode>(Types::_NULL,@2),$4));}

start	: /* empty */
        | start cd  {   if(!env.errors.hasCritical()){
                            if(!env.addClass(*$2)){
                                error(@2,"Duplicate class definition for \'" + $2->getType() + "\'.");
                            }
                        }
                    }

 /*** END Cool GRAMMAR RULES ***/


%% /*** Additional Code ***/
void Cool::Parser::error(const Parser::location_type& l, const std::string& m)
{
    env.errors.registerError(l,m);
}