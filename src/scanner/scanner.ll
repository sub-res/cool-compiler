/* $Id: scanner.ll 44 2008-10-23 09:03:19Z tb $ -*- mode: c++ -*- */
/** \file scanner.ll Define the Cool Flex lexical scanner */

%{ /*** C/C++ Declarations ***/

#include <string>
#include <map>
#include "Scanner.h"
#include "StringContainer.h"

/* import the parser's token type into a local typedef */
typedef Cool::Parser::token token;
typedef Cool::Parser::token_type token_type;

/* By default yylex returns int, we use token_type. Unfortunately yyterminate
 * by default returns 0, which is not of token_type. */
#define yyterminate() return token::ENDOF

/* This disables inclusion of unistd.h, which is not available under Visual C++
 * on Win32. The C++ scanner uses STL streams instead. */
#define YY_NO_UNISTD_H

std::string repl_esc_chars( char * &,int);

bool has_esc_nul;
bool has_esc_bspace;
bool has_esc_return;
bool has_esc_ffeed;
bool has_esc_bslash;
bool has_esc_dquote;
%}

/*** Flex Declarations and Options ***/

/* enable c++ scanner class generation */
%option c++

/* change the name of the scanner class. results in "CoolFlexLexer" */
%option prefix="Cool"

/* the manual says "somewhat more optimized" */
%option batch

/* enable scanner to generate debug output. disable this for release
 * versions. */
%option debug

/* no support for include files is planned */
%option yywrap nounput 

/*%option outfile="Scanner.cpp"*/

/* enables the use of start condition stacks */
%option stack

/*enable linenumber tracking*/


/* The following paragraph suffices to track locations accurately. Each time
 * yylex is invoked, the begin position is moved onto the end position. */
%{
#define YY_USER_ACTION  yylloc->columns(yyleng);
%}

alpha						[_a-zA-Z]
obj						[a-z][_a-zA-Z0-9]*
type						[A-Z][_a-zA-Z0-9]*
num						[0-9]+

%x COMMENT STRING CSTRING
%% /*** Regular Expressions Part ***/

 /* code to place at the beginning of yylex() */
%{
    // reset location
    yylloc->step();
%}

 /*** BEGIN LEXER RULES***/
					int c = 0;
" "|"\t"|"\r"                           {yylloc->step();/*whitespace*/}
\n                                      {yylloc->lines(yyleng); yylloc->step();}
"+"					{return token::PLUS;}
"-"					{return token::MINUS;}
"*"					{return token::AST;}
"/"					{return token::FSLASH;}
"<"					{return token::LT;}
"<="					{return token::LTEQ;}
"=="					{return token::EQ;}
"=>"                                    {return token::DO;}
"{"					{return token::BOPEN;}
"}"					{return token::BCLOSE;}
"("					{return token::POPEN;}
")"					{return token::PCLOSE;}
"."					{return token::PERIOD;}
","					{return token::COMMA;}
":"					{return token::COLON;}
";"					{return token::SCOLON;}
"="					{return token::ASSGN;}
"!"					{return token::BANG;}
"case"					{return token::KCASE;}
"class"					{return token::KCLASS;}
"def"					{return token::KDEF;}
"else"					{return token::KELSE;}
"extends"				{return token::KEXT;}
"if"					{return token::KIF;}
"match"					{return token::KMATCH;}
"native"				{return token::KNAT;}
"new"					{return token::KNEW;}
"null"					{return token::KNULL;}
"override"				{return token::KOVER;}
"super"					{return token::KSUPER;}
"this"					{return token::KTHIS;}
"var"					{return token::KVAR;}
"while"					{return token::KWHILE;}
"abstract"|"catch"|"do"|"final"|"finally"|"for"|"forSome"|"implicit"|"import"|"lazy"|"object"|"package"|"private"|"protected"|"requires"|"return"|"sealed"|"throw"|"trait"|"try"|"type"|"val"|"with"|"yield" {registerError(yylloc,"Illegal keyword"); yyterminate();}
\/\/.*  				{yylloc->step();/*comment*/}
"/*"					{BEGIN COMMENT;yyless(yyleng-2);}
"true"|"false"				{ yylval->boolVal = (yyleng == 4); return token::TBOOL;}						
{obj}					{yylval->strVal = &StringContainer::get(yytext); return token::OBJID;}
{type}					{yylval->strVal = &StringContainer::get(yytext); return token::TYPEID;}
\"\"\"					{BEGIN CSTRING;}
\"					{BEGIN STRING;}
{num}					{if(strlen(yytext) > 1 && yytext[0] == '0'){registerError(yylloc,"Integer starting with zero"); yyterminate();}else { 
yylval->intVal = atoi(yytext);
return token::TINT; } }
.					{registerError(yylloc,"Unrecognized syntax");yyterminate();}
<COMMENT>"*/"				{--c; if(c==0){BEGIN 0;}else{yymore();}}
<COMMENT>"/*"				{++c;yymore(); }
<COMMENT>\n                             {yylloc->lines(1);yymore();}
<COMMENT>.				{yymore();}
<COMMENT><<EOF>>			{registerError(yylloc,"End of file, unterminated comment"); yyterminate();}
<STRING>\\0				{has_esc_nul = true;yymore();}
<STRING>\\t				{yymore();}
<STRING>\\b		        	{has_esc_bspace = true;yymore(); }
<STRING>\\\\				{has_esc_bslash = true;yymore();}
<STRING>\\\"				{has_esc_dquote = true;yymore();}
<STRING>\\r				{has_esc_return = true;yymore();}
<STRING>\\f				{has_esc_ffeed = true; yymore();}
<STRING>\\[^0btnrf\"\\]                 {std::string a(yytext);registerError(yylloc,"Illegal escape sequence in \'" + a + "\'");yyterminate(); }
<STRING><<EOF>>				{registerError(yylloc,"End of file, unterminated string");yyterminate(); }
<STRING><<EOL>>				{registerError(yylloc,"End of file, unterminated string");yyterminate(); }
<STRING>\"				{BEGIN 0; yytext[yyleng-1] ='\0'; std::string value = repl_esc_chars(yytext,yyleng); yylval->strVal = &StringContainer::get(std::string(value)); return token::TSTR;}
<STRING>.				{yymore();}
<CSTRING>\"\"\"\"?\"?			{BEGIN 0;char newS[yyleng-2]; newS[yyleng-3] = '\0';for(int i = 0; i < yyleng-3 ; ++i){newS[i] = yytext[i];} yylval->strVal = &StringContainer::get(std::string(newS)); return token::TCSTR;}
<CSTRING>\n                             {yylloc->lines(1);yymore();}
<CSTRING>.				{yymore();}
<CSTRING>\Q"""\E[.\n]*\Q"""\E           {registerError(yylloc,"Illegal triple-quote string");yyterminate();}
<CSTRING><<EOF>>			{registerError(yylloc,"End of file, unterminated string");yyterminate();}


%% /*** Additional Code ***/


namespace Cool {


Scanner::Scanner(Errors&e, std::istream* in,
		 std::ostream* out)
    : CoolFlexLexer(in, out), errors(&e)
{
}

Scanner::~Scanner()
{
}

void Scanner::registerError(location *yylloc, std::string message)
{
errors->registerError(*yylloc, message);
}

void Scanner::set_debug(bool b)
{
    yy_flex_debug = b;
}



}

/* This implementation of CoolSpaceFlexLexer::yylex() is required to fill the
 * vtable of the class CoolSpaceFlexLexer. We define the scanner's main yylex
 * function via YY_DECL to reside in the Scanner class instead. */

#ifdef yylex
#undef yylex
#endif

int CoolFlexLexer::yylex()
{
    std::cerr << "in CoolspaceFlexLexer::yylex() !" << std::endl;
    return 0;
}

/* When the scanner receives an end-of-file indication from YY_INPUT, it then
 * checks the yywrap() function. If yywrap() returns false (zero), then it is
 * assumed that the function has gone ahead and set up `yyin' to point to
 * another input file, and scanning continues. If it returns true (non-zero),
 * then the scanner terminates, returning 0 to its caller. */

int CoolFlexLexer::yywrap()
{
    return 1;
}


void replace(std::string & a,const std::string b,const std::string c)
{
    int pos ;
    do
    {
        pos = a.find(b);
        if (pos!=-1)  a.replace(pos, b.length(), c);
    }
    while (pos!=-1);
}

void esc_backspace(std::string & a)
{
	const std::string BS = "\\b";
	
	while(a.size()>=2){
	if(a.substr(0,2) == BS)
	{
		a= a.substr(2,a.size()-2);
	}
	else if(a.substr(a.size() -2,2)==BS){
		a = a.substr(0,a.size()-2);
	}else{
		break;
		}		

	}
    int pos;
    do
    {
    	
        pos = a.find("\\b");
        
        if (pos!=-1){ 
int l = a.length();
		std::string b = a.substr(pos+2);
 		a.replace(pos-1, b.length(),b);
a.replace(l-3,3,"");
	}
    }
    while (pos!=-1);
}

std::string repl_esc_chars(char * & yytext, int length)
{
std::string s(yytext);/*
if(has_esc_nul){
replace(s,"\\0","NUL"); //NUL
has_esc_nul = false;
}
if(has_esc_ffeed){
replace(s,"\\f","\f"); //form feed
has_esc_ffeed = false;}



if(has_esc_bslash){
replace(s,"\\\\","\\"); // backslash
has_esc_bslash = false;
}

if(has_esc_return){
replace(s,"\\r","\r");  //return
has_esc_return = false;}



if(has_esc_dquote){
replace(s,"\\\"","\"");// quote
has_esc_dquote = false;} 



*/
return s;
}

