// This source file is part of Epoxy licensed under the MIT License.
// See LICENSE.md file for details.

%skeleton "lalr1.cc"
%require "3.0.4"

%defines "parser.h"
%output "parser.cc"

%define api.parser.class {Parser}
%define api.prefix {epoxy_}
%define api.namespace {epoxy}
%define api.token.constructor
%define api.value.type variant
%define parse.trace
%define parse.error verbose
%define api.token.prefix {epoxy_TOKEN_}

%param { epoxy::Driver& driver }
%param { void *scanner }

%no-lines
%locations

%initial-action {

};

%code requires {
#include "impeller/stator/epoxy/driver.h"
#include "impeller/stator/epoxy/types.h"
}

%code {
#include "impeller/stator/epoxy/decls.h"
}

%token
  END 0                   "<end of contents>"

  NAMESPACE               "namespace"
  CLASS                   "class"
  STRUCT                  "struct"
  OPAQUE                  "opaque"
  FUNCTION                "function"
  ENUM                    "enum"

  INVALID_TOKEN           "<invalid token>"

  SEMI_COLON              ";"
  CURLY_LEFT              "{"
  CURLY_RIGHT             "}"
  PAREN_LEFT              "("
  PAREN_RIGHT             ")"
  COMMA                   ","

  VOID                    "void"

  BOOL                    "bool"

  INT_8                   "int8"
  INT_16                  "int16"
  INT_32                  "int32"
  INT_64                  "int64"

  UINT_8                  "uint8"
  UINT_16                 "uint16"
  UINT_32                 "uint32"
  UINT_64                 "uint64"

  DOUBLE                  "double"
  FLOAT                   "float"

  ARROW                   "->"
  STAR                    "*"
  ;

%token <std::string>
  IDENTIFIER      "<identifier>"

%type <epoxy::Namespace> Namespace
%type <epoxy::NamespaceItems> NamespaceItems
%type <epoxy::NamespaceItem> NamespaceItem
%type <epoxy::Function> Function
%type <epoxy::Functions> Functions
%type <epoxy::Variable> Variable
%type <std::vector<epoxy::Variable>> ArgumentList
%type <std::vector<epoxy::Variable>> VariableList
%type <epoxy::Primitive> Primitive
%type <epoxy::Struct> Struct
%type <epoxy::Opaque> Opaque
%type <epoxy::Enum> Enum
%type <std::vector<std::string>> IdentifierList
%type <std::variant<Primitive, std::string>> PrimitiveOrIdentifier

%start SourceFile

%%

SourceFile
  : NamespaceList
  | %empty
  ;

NamespaceList
  : Namespace                 { driver.AddNamespace($1); }
  | NamespaceList Namespace   { driver.AddNamespace($2); }
  ;

Namespace
  : NAMESPACE IDENTIFIER CURLY_LEFT NamespaceItems CURLY_RIGHT { $$ = epoxy::Namespace{$2, $4}; }
  | NAMESPACE IDENTIFIER CURLY_LEFT                CURLY_RIGHT { $$ = epoxy::Namespace{$2, {}}; }
  ;

NamespaceItems
  : NamespaceItem                  { $$ = {$1}; }
  | NamespaceItems NamespaceItem   { $$ = $1; $$.push_back($2); }
  ;

NamespaceItem
  : Function  { $$ = $1; }
  | Struct    { $$ = $1; }
  | Opaque    { $$ = $1; }
  | Enum      { $$ = $1; }
  ;

Enum
  : ENUM IDENTIFIER CURLY_LEFT                CURLY_RIGHT { $$ = epoxy::Enum{$2, {}}; }
  | ENUM IDENTIFIER CURLY_LEFT IdentifierList CURLY_RIGHT { $$ = epoxy::Enum{$2, $4}; }
  ;

IdentifierList
  : IDENTIFIER COMMA                       { $$ = {$1}; }
  | IDENTIFIER                             { $$ = {$1}; }
  | IdentifierList IDENTIFIER COMMA        { $$ = $1; $$.push_back($2); }
  | IdentifierList IDENTIFIER              { $$ = $1; $$.push_back($2); }
  ;

Function
  : FUNCTION IDENTIFIER PAREN_LEFT ArgumentList PAREN_RIGHT ARROW PrimitiveOrIdentifier      { $$ = epoxy::Function{$2, $4, $7, false}; }
  | FUNCTION IDENTIFIER PAREN_LEFT ArgumentList PAREN_RIGHT ARROW PrimitiveOrIdentifier STAR { $$ = epoxy::Function{$2, $4, $7, true}; }
  | FUNCTION IDENTIFIER PAREN_LEFT ArgumentList PAREN_RIGHT                                  { $$ = epoxy::Function{$2, $4, epoxy::Primitive::kVoid, false}; }
  | FUNCTION IDENTIFIER PAREN_LEFT              PAREN_RIGHT ARROW PrimitiveOrIdentifier      { $$ = epoxy::Function{$2, {}, $6, false}; }
  | FUNCTION IDENTIFIER PAREN_LEFT              PAREN_RIGHT ARROW PrimitiveOrIdentifier STAR { $$ = epoxy::Function{$2, {}, $6, true}; }
  | FUNCTION IDENTIFIER PAREN_LEFT              PAREN_RIGHT                                  { $$ = epoxy::Function{$2, {}, epoxy::Primitive::kVoid, false}; }
  ;

PrimitiveOrIdentifier
  : Primitive       { $$ = $1; }
  | IDENTIFIER      { $$ = $1; }
  ;

ArgumentList
  : Variable                        { $$ = {$1}; }
  | ArgumentList COMMA Variable     { $$ = $1; $$.push_back($3); }
  ;

Struct
  : STRUCT IDENTIFIER CURLY_LEFT VariableList  CURLY_RIGHT { $$ = epoxy::Struct{$2, $4}; }
  | STRUCT IDENTIFIER CURLY_LEFT               CURLY_RIGHT { $$ = epoxy::Struct{$2, {}}; }
  ;

Opaque
  : OPAQUE IDENTIFIER                                  { $$ = epoxy::Opaque{$2}; }
  | OPAQUE IDENTIFIER CURLY_LEFT           CURLY_RIGHT { $$ = epoxy::Opaque{$2}; }
  | OPAQUE IDENTIFIER CURLY_LEFT Functions CURLY_RIGHT { $$ = epoxy::Opaque{$2, $4}; }
  ;

Functions
  : Function                     { $$ = {$1}; }
  | Functions Function           { $$ = $1; $$.push_back($2); }
  ;

Variable
  : Primitive        IDENTIFIER  { $$ = epoxy::Variable{$1, $2, false}; }
  | Primitive  STAR  IDENTIFIER  { $$ = epoxy::Variable{$1, $3, true};  }
  | IDENTIFIER       IDENTIFIER  { $$ = epoxy::Variable{$1, $2, false}; }
  | IDENTIFIER STAR  IDENTIFIER  { $$ = epoxy::Variable{$1, $3, true};  }
  ;

VariableList
  : Variable SEMI_COLON              { $$ = {$1}; }
  | VariableList Variable SEMI_COLON { $$ = $1; $$.push_back($2); }
  ;

Primitive
  : VOID       { $$ = epoxy::Primitive::kVoid; }
  | BOOL       { $$ = epoxy::Primitive::kBool; }
  | INT_8      { $$ = epoxy::Primitive::kInt8; }
  | INT_16     { $$ = epoxy::Primitive::kInt16; }
  | INT_32     { $$ = epoxy::Primitive::kInt32; }
  | INT_64     { $$ = epoxy::Primitive::kInt64; }
  | UINT_8     { $$ = epoxy::Primitive::kUnsignedInt8; }
  | UINT_16    { $$ = epoxy::Primitive::kUnsignedInt16; }
  | UINT_32    { $$ = epoxy::Primitive::kUnsignedInt32; }
  | UINT_64    { $$ = epoxy::Primitive::kUnsignedInt64; }
  | DOUBLE     { $$ = epoxy::Primitive::kDouble; }
  | FLOAT      { $$ = epoxy::Primitive::kFloat; }
  ;

%%

void epoxy::Parser::error(const epoxy::Parser::location_type& loc,
                          const std::string& message) {
  driver.ReportParsingError(loc, message);
}
