
%option noyywrap

%{
   int lineNumber =1;
   int colNumber = 0;

   enum token
   {
        prog,
        IDENT,
        STRINGLIT,
        INTLIT,
        INTEGER, STRING,
        IF, WHILE, RETURN, PRINT, ELSE,
        EQ, NEQ, LT, GT, LEQ, GEQ,
        PLUS, MINUS,
        MULT, DIV,
        UMINUS,
        OPENPAR, CLOSEPAR,
        SEMICOLON, COMA,
        OPENCURLY, CLOSECURLY,
        ERROR
   };
%}

DIGIT   [0-9]
CHAR    [a-zA-Z]
STRING  "String"
INT     "Integer"
OPENPAR "("
CLOSEPAR ")"
IF "if"
ELSE "else"
RET "return"
PRINT "print"
WHILE "while"
OPENCURLY "{"
CLOSECURLY "}"
COMA ","
SEMI ";"
GT ">"
GE ">="
EQ "="
LE "<="
LT "<"
PLUS "+"
MINUS "-"
MUL "*"
DIV "/"
ID {CHAR}({CHAR}|{DIGIT})*
STRVAL "\""[^"]*"\""
INTVAL {DIGIT}+





%%


{INTVAL}    {

   printf(" %4d: %4d: INTVAL %s \n",lineNumber,colNumber,yytext);
   colNumber +=yyleng;
            }
{OPENPAR} {
   printf(" %4d: %4d: OPENPAR %s \n",lineNumber,colNumber,yytext);
   colNumber +=yyleng;
}
{CLOSEPAR} {
   printf(" %4d: %4d: CLOSEPAR %s \n",lineNumber,colNumber,yytext);
   colNumber +=yyleng;
}
{OPENCURLY} {
   printf(" %4d: %4d: OPENCURLY %s \n",lineNumber,colNumber,yytext);
   colNumber +=yyleng;
}
{CLOSECURLY} {
   printf(" %4d: %4d: CLOSECURLY %s \n",lineNumber,colNumber,yytext);
   colNumber +=yyleng;
}
{RET} {
   printf(" %4d: %4d: RET %s \n",lineNumber,colNumber,yytext);
   colNumber +=yyleng;
}

{PRINT} {
   printf(" %4d: %4d: PRINT %s \n",lineNumber,colNumber,yytext);
   colNumber +=yyleng;
}

{SEMI} {
   printf(" %4d: %4d: SEMI %s \n",lineNumber,colNumber,yytext);
   colNumber +=yyleng;
}


{STRVAL} {
   printf(" %4d: %4d: STRVAL %s \n",lineNumber,colNumber,yytext);
   colNumber +=yyleng;
}

{STRING} {
   printf(" %4d: %4d: STRING %s \n",lineNumber,colNumber,yytext);
   colNumber +=yyleng;
}

{INT} {
   printf(" %4d: %4d: INT %s \n",lineNumber,colNumber,yytext);
   colNumber +=yyleng;
}

{ID} {
   printf(" %4d: %4d: ID %s \n",lineNumber,colNumber,yytext);
   colNumber +=yyleng;
}

\n {
    lineNumber++;
    colNumber =0;
}

. {/* Skip */
    colNumber++;
 }
%%

int main( int argc, char **argv )       
{
   ++argv, --argc;  /* skip over program name */
   if ( argc > 0 )
      yyin = fopen( argv[0], "r" );
   else
      yyin = stdin;
   
   yylex();

}
