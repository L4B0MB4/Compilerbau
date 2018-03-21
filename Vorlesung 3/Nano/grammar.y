 /*
     nanoLang grammar

     Copyright 2017 by Stephan Schulz, schulz@eprover.org.

     This code is released under the GNU General Public Licence
     Version 2.
 */

%{
   #include <stdio.h>
   #include <stdlib.h>

   extern int yyerror(const char* err);
   extern int yylex(void);
   extern FILE *yyin;

   int yyerror(const char* err)
   {
       printf("%s \n",err);
       return 0;
   }
%}

%locations

%define parse.error verbose

%start prog

%token IDENT
%token STRINGLIT
%token INTLIT
%token INTEGER STRING
%token IF WHILE RETURN PRINT ELSE
%token EQ NEQ LT GT LEQ GEQ
%left  PLUS MINUS
%left  MULT DIV
%right UMINUS
%token OPENPAR CLOSEPAR
%token SEMICOLON COMA
%token OPENCURLY CLOSECURLY

%token ERROR


%%

prog: /* Nothing */
    | prog def
;

def: vardef
   | fundef
;

vardef: type idlist SEMICOLON
;

idlist: IDENT
      | idlist COMA IDENT

fundef: type IDENT OPENPAR params CLOSEPAR body
;

type: STRING
    | INTEGER
;

params: /* empty */
      | paramlist
;

paramlist: param
         | paramlist COMA param
;

param: type IDENT
;

body: OPENCURLY vardefs stmts CLOSECURLY
;

vardefs: /* empty */
       | vardefs vardef
;


stmts: /* empty */
     | stmts stmt
;

stmt: while_stmt
    | if_stmt
    | ret_stmt
    | print_stmt
    | assign
    | funcall_stmt
;

while_stmt: WHILE OPENPAR boolexpr CLOSEPAR body
;

if_stmt: IF OPENPAR boolexpr CLOSEPAR body
       | IF OPENPAR boolexpr CLOSEPAR body ELSE body
;

ret_stmt: RETURN expr SEMICOLON
;

print_stmt: PRINT expr SEMICOLON
;

assign: IDENT EQ expr SEMICOLON
;

funcall_stmt: funcall SEMICOLON
;

boolexpr: expr EQ expr
        | expr NEQ expr
        | expr LT expr
        | expr GT expr
        | expr LEQ expr
        | expr GEQ expr
;

expr: funcall
    | INTLIT
    | IDENT
    | STRINGLIT
    | OPENPAR expr CLOSEPAR
    | expr PLUS expr
    | expr MINUS expr
    | expr MULT expr
    | expr DIV expr
    | MINUS expr %prec UMINUS
;

funcall: IDENT OPENPAR args CLOSEPAR
;

args: /* empty */
    | arglist
;

arglist: expr
       | arglist COMA expr
;


%%
