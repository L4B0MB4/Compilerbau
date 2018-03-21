/*-----------------------------------------------------------------------

File  : nanolex_drv.h

Author: Stephan Schulz (schulz@eprover.org)

Contents

  Header file for the nanoLex scanner. This includes everything needed
  for the interface between flex lexer and driver/parser.

  Copyright 2015-2018 by the author.
  This code is released under the GNU General Public Licence.
  See the file COPYING in the main CLIB directory for details.
  Run "eprover -h" for contact information.

-----------------------------------------------------------------------*/

#ifndef NANOLEX_DRV

#define NANOLEX_DRV

#include <stdio.h>


/*---------------------------------------------------------------------*/
/*                    Data type declarations                           */
/*---------------------------------------------------------------------*/

   /* List of possible tokens. If used with Bison, this list comes
    * from the Bison-generated something.tab.h. By default, yylex()
    * will return 0 at the end of the file, so we give that a name as
    * well.  */

enum yytokentype
{
   EOFTOK,
   OPENPAR,
   CLOSEPAR,
   MULT,
   DIV,
   PLUS,
   MINUS,
   EQ,
   NEQ,
   LT,
   GT,
   LEQ,
   GEQ,
   OPENCURLY,
   CLOSECURLY,
   SEMICOLON,
   COMA,
   IDENT,
   STRINGLIT,
   INTLIT,
   INTEGER,
   STRING,
   IF,
   WHILE,
   RETURN,
   PRINT,
   ERROR
};


typedef struct
{
   int  line;
   int  col;
   char *lexeme;
}TokenCell, *Token_p;


/* YYSTYPE is used for semantic values by flex/bison - we define it
 * as a synonym for TokenCell. */

typedef TokenCell YYSTYPE;

int yylex(void);



/*---------------------------------------------------------------------*/
/*                Exported Functions and Variables                     */
/*---------------------------------------------------------------------*/


/* Provided for sake of the lexer */
extern YYSTYPE yylval;

/* Required by the lexer */
extern FILE* yyin;


#endif

/*---------------------------------------------------------------------*/
/*                        End of File                                  */
/*---------------------------------------------------------------------*/
