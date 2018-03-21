 /* -*- Mode: c -*-

     Lexer for a minimal programming language

     Copyright 2017 by Stephan Schulz, schulz@eprover.org.

     This code is released under the GNU General Public Licence
     Version 2.
 */

   /* We don't provide a handler for EOF - we only compile one file. */

%option noyywrap

   /* We want to keep track of linen numbers automatically. */

%option yylineno

%{
#include <string.h>

   /* Token definitions come from nanolex_drv.h. */
#include "nanolex_drv.h"


int yycol = 0;

  /* YY_USER_ACTIOn is automatically executed whenever a pattern is
   * matched, and before the rule-specific body is executed. Here, we
   * use the same action for every toke: Fill the TokCell, update
   * yycol. Only the returned token value is different for each token. */
#define YY_USER_ACTION \
   {yylval.line = yylineno; yylval.col=yycol; yylval.lexeme=strdup(yytext);yycol += yyleng;}

%}


DIGIT      [0-9]
ALPHA      [a-zA-Z]
ALNUM      {ALPHA}|{DIGIT}

INT        {DIGIT}+
ID         {ALPHA}{ALNUM}*


%%

"("  {return OPENPAR;}
")"  {return CLOSEPAR;}
"*"  {return MULT;}
"/"  {return DIV;}
"+"  {return PLUS;}
"-"  {return MINUS;}
"="  {return EQ;}
"!=" {return NEQ;}
">"  {return GT;}
"<"  {return LT;}
">=" {return GEQ;}
"<=" {return LEQ;}
"{"  {return OPENCURLY;}
"}"  {return CLOSECURLY;}
";"  {return SEMICOLON;}
","  {return COMA;}


Integer {return INTEGER;}
String  {return STRING;}
if      {return IF;}
while   {return WHILE;}
return  {return RETURN;}
print   {return PRINT;}

{ALPHA}{ALNUM}* {return IDENT;}

\"[^"]*\"       {return STRINGLIT;}

{DIGIT}+        {return INTLIT;}


#[^\n]* { /* Eat comments */}


[\n]    {yycol = 0;}

[\r\t ] { /* Skip  whitespace*/ }


 /* Everything else is an invalid character. */
.   {return ERROR;}

%%


#include <stdlib.h>
#include "nanolex_drv.h"

/*---------------------------------------------------------------------*/
/*                        Global Variables                             */
/*---------------------------------------------------------------------*/

/* Names of the tokens for nice output. */

char* tokname[] =
{
   "End Of File",
   "OPENPAR",
   "CLOSEPAR",
   "MULT",
   "DIV",
   "PLUS",
   "MINUS",
   "EQ",
   "NEQ",
   "LT",
   "GT",
   "LEQ",
   "GEQ",
   "OPENCURLY",
   "CLOSECURLY",
   "SEMICOLON",
   "COMA",
   "IDENT",
   "STRINGLIT",
   "INTLIT",
   "INTEGER",
   "STRING",
   "IF",
   "WHILE",
   "RETURN",
   "PRINT",
   "ERROR"
};


/* yylval is the semantic value, used by the parser, but set by the
 * lexer. The variable is provided by the parser side. */

YYSTYPE yylval;


/*---------------------------------------------------------------------*/
/*                      Forward Declarations                           */
/*---------------------------------------------------------------------*/


/*---------------------------------------------------------------------*/
/*                         Internal Functions                          */
/*---------------------------------------------------------------------*/



/*---------------------------------------------------------------------*/
/*                         Exported Functions                          */
/*---------------------------------------------------------------------*/


/*-----------------------------------------------------------------------
//
// Function: print_tok()
//
//   Print a token, defined by token encoding and a corresponding
//   TokenCell with meta-information.
//
// Global Variables: -
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void print_tok(int tok, TokenCell curtok)
{
   printf("<%5d:%3d: %s[%d]=\"%s\">\n",
          curtok.line,
          curtok.col,
          tokname[tok],
          tok,
          curtok.lexeme);
}



/*-----------------------------------------------------------------------
//
// Function: main()
//
//   Main function of the driver: Open input, then call yylex as long
//   as tokens are available. Stop when 0 (End of File) is returned.
//
// Global Variables: -
//
// Side Effects    : Triggers reading of input.
//
/----------------------------------------------------------------------*/


int main( int argc, char **argv )
{
   int tok;

   ++argv, --argc;  /* skip over program name */
   if ( argc > 0 )
      yyin = fopen( argv[0], "r" );
   else
      yyin = stdin;
  yyparse();
   while((tok=yylex())!=EOFTOK)
   {
      //print_tok(tok, yylval);
      /* Our token variable has a strdup()ed string that we need to
         free() again - C is manually memory managed! */
      free(yylval.lexeme);
   }
}