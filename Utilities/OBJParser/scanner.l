
%{
	#include <iostream>
	#include <cstdlib>
	#include "scanner.h"
	#include "ObjParser.h"
	#include "parser.tab.h"
	#include "location.hh"

	using namespace std;

	// Original yyterminate() macro returns int. Since we're using Bison 3 variants
	// as tokens, we must redefine it to change type from `int` to `Parser::semantic_type`
	#define yyterminate() ObjParser::Parser::make_END(ObjParser::location());

	// This will track current scanner location.
	// Action is called when length of the token is known.
	#define YY_USER_ACTION m_driver.increaseLocation(yyleng);

%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="ObjParser_"

%x comment
ws      [ \t\n\r]+

%%

"#"            { BEGIN(comment); }
<comment>"\n" { BEGIN(INITIAL); }
<comment>.    { }


{ws}									{}


[0-9]+												{ return ObjParser::Parser::make_INTEGER(stoul(yytext), ObjParser::location());}
[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?				{ return ObjParser::Parser::make_REAL(std::stof(yytext), ObjParser::location());}
v													{ return ObjParser::Parser::make_POSITION(yytext, ObjParser::location());}
vt													{ return ObjParser::Parser::make_TEXCOORD(yytext, ObjParser::location());}
vn													{ return ObjParser::Parser::make_NORMAL(yytext, ObjParser::location());}
vp													{ return ObjParser::Parser::make_POINT(yytext, ObjParser::location());}
o													{ return ObjParser::Parser::make_OBJECT(yytext, ObjParser::location());}
g													{ return ObjParser::Parser::make_OBJECT(yytext, ObjParser::location());}
s													{ return ObjParser::Parser::make_S(yytext, ObjParser::location());}
f													{ return ObjParser::Parser::make_FACE(yytext, ObjParser::location());}

mtllib												{ return ObjParser::Parser::make_MTLLIB(yytext, ObjParser::location());}
usemtl												{ return ObjParser::Parser::make_USEMTL(yytext, ObjParser::location());}

off													{ return ObjParser::Parser::make_BOOLEAN(false, ObjParser::location());}
on													{ return ObjParser::Parser::make_BOOLEAN(true, ObjParser::location());}

[a-zA-Z0-9._:-]+									{ std::cout << yytext << std::endl; return ObjParser::Parser::make_NAME(yytext, ObjParser::location());}

\/													{ return ObjParser::Parser::make_SEP("true", ObjParser::location());}

<<EOF>>     { return yyterminate(); }

.													{}


%%
