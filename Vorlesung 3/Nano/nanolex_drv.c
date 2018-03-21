/*-----------------------------------------------------------------------

File  : nanolex_drv.c

Author: Stephan Schulz (schulz@eprover.org)

Contents

  Driver for the nanolex parser. This implements all the parts of the
  interface normally provided by the Bison/YACC parser. It shows how
  the token stream can be extracted from the lexer manually.

  Copyright 2017, 2018 by the author.
  This code is released under the GNU General Public Licence.


-----------------------------------------------------------------------*/

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
      print_tok(tok, yylval);
      /* Our token variable has a strdup()ed string that we need to
         free() again - C is manually memory managed! */
      free(yylval.lexeme);
   }
}


/*---------------------------------------------------------------------*/
/*                        End of File                                  */
/*---------------------------------------------------------------------*/
