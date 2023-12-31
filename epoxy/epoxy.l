/*
  This source file is part of Epoxy licensed under the MIT License.
  See LICENSE.md file for details.
*/

%{

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>

#include "parser/epoxy/parser.h"
#include "impeller/stator/epoxy/types.h"
#include "impeller/stator/epoxy/decls.h"

#define CURRENT_LOC driver.GetCurrentLocation()

#define YY_USER_ACTION driver.BumpCurrentLocation(yytext);

%}

%option 8bit noyywrap nounput batch debug noinput never-interactive reentrant
%option outfile="lexer.cc"
%option header-file="lexer.h"
%option prefix="epoxy_"

D                       [0-9]
L                       [a-zA-Z_]
A                       [a-zA-Z_0-9]
WS                      [ \t\v\n\f]

%%

<<EOF>>                return epoxy::Parser::make_END(CURRENT_LOC);

"//".*                  {  /* Comment Consumed */  };

"namespace"            return epoxy::Parser::make_NAMESPACE(CURRENT_LOC);
"class"                return epoxy::Parser::make_CLASS(CURRENT_LOC);
"enum"                 return epoxy::Parser::make_ENUM(CURRENT_LOC);
"struct"               return epoxy::Parser::make_STRUCT(CURRENT_LOC);
"opaque"               return epoxy::Parser::make_OPAQUE(CURRENT_LOC);
"function"             return epoxy::Parser::make_FUNCTION(CURRENT_LOC);

"void"                 return epoxy::Parser::make_VOID(CURRENT_LOC);
"bool"                 return epoxy::Parser::make_BOOL(CURRENT_LOC);
"int8"                 return epoxy::Parser::make_INT_8(CURRENT_LOC);
"int16"                return epoxy::Parser::make_INT_16(CURRENT_LOC);
"int32"                return epoxy::Parser::make_INT_32(CURRENT_LOC);
"int64"                return epoxy::Parser::make_INT_64(CURRENT_LOC);
"uint8"                return epoxy::Parser::make_UINT_8(CURRENT_LOC);
"uint16"               return epoxy::Parser::make_UINT_16(CURRENT_LOC);
"uint32"               return epoxy::Parser::make_UINT_32(CURRENT_LOC);
"uint64"               return epoxy::Parser::make_UINT_64(CURRENT_LOC);
"double"               return epoxy::Parser::make_DOUBLE(CURRENT_LOC);
"float"                return epoxy::Parser::make_FLOAT(CURRENT_LOC);

";"                    return epoxy::Parser::make_SEMI_COLON(CURRENT_LOC);
"{"                    return epoxy::Parser::make_CURLY_LEFT(CURRENT_LOC);
"}"                    return epoxy::Parser::make_CURLY_RIGHT(CURRENT_LOC);
"("                    return epoxy::Parser::make_PAREN_LEFT(CURRENT_LOC);
")"                    return epoxy::Parser::make_PAREN_RIGHT(CURRENT_LOC);
"->"                   return epoxy::Parser::make_ARROW(CURRENT_LOC);
","                    return epoxy::Parser::make_COMMA(CURRENT_LOC);
"*"                    return epoxy::Parser::make_STAR(CURRENT_LOC);

{L}{A}*                return epoxy::Parser::make_IDENTIFIER(yytext, CURRENT_LOC);

{WS}+                  {  /* Whitespace Consumed */  }
.                      return epoxy::Parser::make_INVALID_TOKEN(CURRENT_LOC);

%%
