%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0"
%defines
%define parser_class_name { Parser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { ObjParser }

%code requires
{
    #include <iostream>
    #include <string>
    #include <vector>
    #include <stdint.h>

    using namespace std;

    namespace ObjParser {
        class Scanner;
        class Interpreter;
    }
}

// Bison calls yylex() function that must be provided by us to suck tokens
// from the scanner. This block will be placed at the beginning of IMPLEMENTATION file (cpp).
// We define this function here (function! not method).
// This function is called only inside Bison, so we make it static to limit symbol visibility for the linker
// to avoid potential linking conflicts.
%code top
{
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
}

%lex-param { ObjParser::Scanner &scanner }
%lex-param { ObjParser::Interpreter &driver }
%parse-param { ObjParser::Scanner &scanner }
%parse-param { ObjParser::Interpreter &driver }
%locations
%define parse.trace
%define parse.error verbose

%define api.token.prefix {TOKEN_}

%type <void*> line
%type <void*> statement
%type <std::vector<std::vector<uint32_t>>> createface
%type <std::string> optname
%type <float> real
%type <float> optreal
%type <float> optreal2
%type <std::vector<uint32_t>> indices

%token <float> REAL
%token <uint32_t> INTEGER
%token <std::string> POSITION
%token <std::string> TEXCOORD
%token <std::string> NORMAL
%token <std::string> POINT
%token <std::string> FACE
%token <std::string> OBJECT
%token <std::string> S
%token <std::string> MTLLIB
%token <std::string> USEMTL
%token <std::string> NAME
%token <bool> BOOLEAN
%token <std::string> SEP
%token END 0 "end of file"

%%


line			: statement									{ driver.increaseRow(); }
				| line statement							{ driver.increaseRow(); }
				;

statement		: OBJECT optname 							{ driver.AddSubMesh($2);}		
				| MTLLIB NAME NAME							{ }
				| POSITION real real real optreal			{ driver.AddPosition(ArfData::Position($2,$3,$4)); }
				| TEXCOORD real real optreal2				{ driver.AddTexCoord(ArfData::TexCoord($2,$3));}
				| NORMAL real real real						{ driver.AddNormal(ArfData::Normal($2,$3,$4)); }
				| POINT real real optreal					{ }
				| USEMTL NAME								{ }					
				| S BOOLEAN									{ }
				| S INTEGER 								{ }
				| FACE createface							{ driver.AddFace(ArfData::Face($2));}
				;	

real			: REAL										{ $$ = $1; }
				| INTEGER									{ $$ = static_cast<float>($1);}
				;

optname			: NAME										{ $$= $1;}
				|/*empty*/									{ $$ = "default";}
				;
					
optreal			: real										{ $$ = $1; }
				| /*empty*/									{ $$ = 1.0f; }
				;
				
optreal2		: real										{ $$ = $1;}
				| /*empty*/									{ $$ = 0.0f;}
				;
				
createface		: indices 									{ $$.push_back($1); }
				| createface indices						{ $$ = $1; $$.push_back($2);}
				;
				
indices			: INTEGER									{ $$.push_back($1 - 1); }
				| indices SEP SEP INTEGER					{ $$ = $1; $$.push_back(UINT32_MAX); $$.push_back($4 - 1);}
				| indices SEP INTEGER						{ $$ = $1; $$.push_back($3 - 1);}		
				;	

%%

// Bison expects us to provide implementation - otherwise linker complains
void ObjParser::Parser::error(const location &loc , const std::string &message) {
        
        // Location should be initialized inside scanner action, but is not in this example.
        // Let's grab location directly from driver class.
	// cout << "Error: " << message << endl << "Location: " << loc << endl;
	
       driver.PutError(message + "\nRow: " + std::to_string(driver.row() + 1) + "\nUnexpected '" + driver.errorCharacter() + "' at location " + std::to_string(driver.location()) + "\n");
}
