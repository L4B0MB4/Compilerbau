 /*
     nanoLang grammar

     Copyright 2017 by Stephan Schulz, schulz@eprover.org.

     This code is released under the GNU General Public Licence
     Version 2.
 */

%{
   
   #include <stdio.h>
   #include <string.h>
   #include "ast.h"

   extern int yyerror(const char* err);
   extern int yylex(void);
   extern FILE *yyin;

   AST_p ast;

   int yyerror(const char* err)
   {
       printf("%s \n",err);
       return 0;
   }
%}

%locations
%define parse.error verbose
%define api.value.type {AST_p}


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
   |  fundef
;

vardef: type idlist SEMICOLON
;

idlist: IDENT
      | idlist COMA IDENT
;

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
    | INTLIT  {$$ = $1;}
    | IDENT   {$$ = $1;}
    | STRINGLIT {$$ = $1;}
    | OPENPAR expr CLOSEPAR {$$ = $2;ASTFree($1);ASTFree($3);}
    | expr PLUS expr  {$$=ASTAlloc2(t_PLUS, NULL, 0, $1, $3);ASTFree($2);}
    | expr MINUS expr {$$=ASTAlloc2(t_MINUS, NULL, 0, $1, $3);ASTFree($2);}
    | expr MULT expr  {$$=ASTAlloc2(t_MULT, NULL, 0, $1, $3);ASTFree($2);}
    | expr DIV expr   {$$=ASTAlloc2(t_DIV, NULL, 0, $1, $3);ASTFree($2);}
    | MINUS expr %prec UMINUS {$$=ASTAlloc2(t_MINUS, NULL, 0, $2, NULL);ASTFree($1);}
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


int main (int argc, char* argv[])
{
  int i;
  int res;
  bool printdot   = false;
  bool printexpr  = true;
  bool printsexpr = false;

   ++argv, --argc;  /* skip over program name */

   if (argc > 0)
   {
      if(strcmp(argv[0], "--dot")==0)
      {
         printdot   = true;
         printexpr  = false;
         printsexpr = false;
         ++argv, --argc;
      }
      else if(strcmp(argv[0], "--sexpr")==0)
      {
         printdot   = false;
         printexpr  = false;
         printsexpr = true;
         ++argv, --argc;
      }
   }

   if ( argc > 0 )
   {
      yyin = fopen( argv[0], "r" );
   }
   else
   {
      yyin = stdin;
   }

   res = yyparse();

   if(res == 0)
   {
      if(printdot)
      {
         DOTASTPrint(stdout, ast);
      }
      if(printexpr)
      {
         ExprASTPrint(stdout, ast);
         printf("\n");
      }
      if(printsexpr)
      {
         SExprASTPrint(stdout, ast);
         printf("\n");
      }
      ASTFree(ast);
   }
   return res;
}
