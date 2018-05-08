
#include "ast.h"

long nodectr=0;

char* ast_name[] =
{
   "nil",
   "prog",
   "def",
   "vardef",
   "fundef",
   "type",
   "params",
   "paramlist",
   "param",
   "body",
   "vardefs",
   "stmts",
   "stmt",
   "while_stmt",
   "if_stmt",
   "else_stmt",
   "ret_stmt",
   "print_stmt",
   "assign",
   "funcall_stmt",
   "boolexpr",
   "expr",
   "funcall",
   "args",
   "arglist",
   "arg",
   "idlist",
   "t_MULT",
   "t_DIV",
   "t_PLUS",
   "t_MINUS",
   "t_EQ",
   "t_NEQ",
   "t_LT",
   "t_GT",
   "t_LEQ",
   "t_GEQ",
   "t_IDENT",
   "t_STRINGLIT",
   "t_INTLIT",
   "t_INTEGER",
   "t_STRING",
   "t_IF",
   "t_WHILE",
   "t_RETURN",
   "t_PRINT",
   "t_dummy"
   "t_ELSE"
};

static void dot_print_child(FILE* out, AST_p father, AST_p child)
{
   if(child)
   {
      fprintf(out, "   ast%ld -> ast%ld\n", father->nodectr, child->nodectr);
      DOTASTNodePrint(out, child);
   }
}

/*-----------------------------------------------------------------------
//
// Function: ASTCellPrint()
//
//   Print an ASTCell to stdout (for debugging).
//
// Global Variables: Reads ast_name[]
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void ASTCellPrint(AST_p cell)
{
   printf("ASTCell(%d, %s, %s, %ld, %p, %p, %p, %p)\n",
          cell->type,
          ast_name[cell->type],
          cell->litval,
          cell->intval,
          cell->child[0],
          cell->child[1],
          cell->child[2],
          cell->child[3]);
}



/*-----------------------------------------------------------------------
//
// Function: ASTEmptyAlloc()
//
//   Allocate an empty ASTNode with no children.
//
// Global Variables:
//
// Side Effects    :
//
/----------------------------------------------------------------------*/

AST_p ASTEmptyAlloc(void)
{
   AST_p ast = ASTCellAlloc();
   int i;

   ast->type = nil;
   ast->litval = NULL;
   ast->nodectr = nodectr++;
   for(i=0; i<MAXCHILD; i++)
   {
      ast->child[i] = NULL;
   }
   return ast;
}


/*-----------------------------------------------------------------------
//
// Function: ASTAlloc()
//
//   Allocate a new ASTCell, initialize it with the given values, and
//   return a pointer to it.
//
// Global Variables: -
//
// Side Effects    : Memory operations
//
/----------------------------------------------------------------------*/


AST_p ASTAlloc(ASTNodeType type, char* litval, long intval, AST_p child0,
               AST_p child1, AST_p child2, AST_p child3)
{
   AST_p ast = ASTEmptyAlloc();

   ast->type = type;
   if(litval)
   {
      ast->litval = strdup(litval);
   }
   ast->intval = intval;
   ast->child[0] = child0;
   ast->child[1] = child1;
   ast->child[2] = child2;
   ast->child[3] = child3;
   ast->yylineno = yylineno;
   ast->yycol = yycol;
   /* ASTCellPrint(ast); */

   return ast;
}


/*-----------------------------------------------------------------------
//
// Function: ASTFree()
//
//   Free an AST.
//
// Global Variables: -
//
// Side Effects    : Memory operations
//
/----------------------------------------------------------------------*/

void ASTFree(AST_p junk)
{
   if(junk)
   {
      free(junk->litval);
      int i;

      for(i=0; junk->child[i]; i++)
      {
         ASTFree(junk->child[i]);
      }
   }
   ASTCellFree(junk);
}



/*-----------------------------------------------------------------------
//
// Function: DOTASTNodePrint()
//
//   Print an AST node and its children in DOT notation.
//
// Global Variables: -
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void DOTASTNodePrint(FILE* out, AST_p ast)
{
   int i;

   switch(ast->type)
   {
   case t_STRINGLIT:
         fprintf(out, "   ast%ld [label=%s]\n",
                 ast->nodectr,
                 ast->litval);
         break;
   case t_IDENT:
   case t_INTLIT:
         fprintf(out, "   ast%ld [label=\"%s  %s \"]\n",
                 ast->nodectr,
                 ast_name[ast->type],
                 ast->litval);
         break;
   default:
         fprintf(out, "   ast%ld [label=\"%s\"]\n",
                 ast->nodectr,
                 ast_name[ast->type]);
         break;
   }
   for(i=0; ast->child[i]; i++)
   {
      dot_print_child(out, ast, ast->child[i]);
   }
}

/*-----------------------------------------------------------------------
//
// Function: DOTASTPrint()
//
//   Print a complete AST as a DOT format graph.
//
// Global Variables: -
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void DOTASTPrint(FILE* out, AST_p ast)
{
   fprintf(out, "digraph ast {\n   ordering=out\n");
   DOTASTNodePrint(out, ast);
   fprintf(out, "}\n");
}

/*-----------------------------------------------------------------------
//
// Function: ExprASTPrint()
//
//   Print an AST representing a nanoLang expression back as an
//   equivalent nanoLang expression.
//
// Global Variables: -
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void ExprASTPrint(FILE* out, AST_p ast)
{
   switch(ast->type)
   {
         case t_INTEGER:
         case t_STRING:
   case t_STRINGLIT:
   case t_IDENT:
   case t_INTLIT:
         fprintf(out, "%s",
                 ast->litval);
         break;
   case t_PLUS:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, "+");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_MULT:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, "*");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_DIV:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, "/");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_LT:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, "<");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_GT:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, ">");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_EQ:
         fprintf(out, "(");
         ExprASTPrint(out, ast->child[0]);
         fprintf(out, "=");
         ExprASTPrint(out, ast->child[1]);
         fprintf(out, ")");
         break;
   case t_MINUS:
         if(ast->child[1]) /* Binary minus */
         {
            fprintf(out, "(");
            ExprASTPrint(out, ast->child[0]);
            fprintf(out, "-");
            ExprASTPrint(out, ast->child[1]);
            fprintf(out, ")");
         }
         else /* Unary minus */
         {
            fprintf(out, "-");
            ExprASTPrint(out, ast->child[0]);
         }
         break;
      case prog:
            if(ast->child[1])
            {
                  fprintf(out, "(");
                  ExprASTPrint(out, ast->child[0]);
                  fprintf(out, ",");
                  ExprASTPrint(out, ast->child[1]);
                  fprintf(out, ")"); 
            }
            break;
      case fundef:
            fprintf(out, "(");
            ExprASTPrint(out, ast->child[0]);
            fprintf(out, " ");
            ExprASTPrint(out, ast->child[1]);
            fprintf(out, ",");
            ExprASTPrint(out, ast->child[2]);
            fprintf(out, ",");
            ExprASTPrint(out, ast->child[3]);
            fprintf(out, ")\n");
            break;
      case params:
            if(ast->child[0])
            {
                  ExprASTPrint(out, ast->child[0]);
            }
            break;
      case paramlist:
            if(ast->child[1])
            {
                  fprintf(out, "(");
                  ExprASTPrint(out, ast->child[0]);
                  fprintf(out, ",");
                  ExprASTPrint(out, ast->child[1]);
                  fprintf(out, ")");
            }
            else{
                  ExprASTPrint(out, ast->child[0]);
            }
            break;
      case param:
            fprintf(out, "(");
            ExprASTPrint(out, ast->child[0]);
            fprintf(out, " ");
            ExprASTPrint(out, ast->child[1]);
            fprintf(out, ")");
            break;
      case body:
            fprintf(out, "(");
            ExprASTPrint(out, ast->child[0]);
            fprintf(out, " ");
            ExprASTPrint(out, ast->child[1]);
            fprintf(out, "\n)");
            break;
      case stmts:
            if(ast->child[0])
            {
                  ExprASTPrint(out, ast->child[0]);
                  fprintf(out, "\n");
                  ExprASTPrint(out, ast->child[1]);
            }
            break;
      case vardefs:
            if(ast->child[0])
            {
                  fprintf(out, "(");
                  ExprASTPrint(out, ast->child[0]);
                  fprintf(out, " ");
                  ExprASTPrint(out, ast->child[1]);
                  fprintf(out, ")");
            }
            break;
      case vardef:
            if(ast->child[0])
            {
                  ExprASTPrint(out, ast->child[0]);
                  fprintf(out, " ");
                  ExprASTPrint(out, ast->child[1]);
                  fprintf(out, ";");
            }
            break;
      case idlist:
            if(ast->child[1])
            {
                  ExprASTPrint(out, ast->child[0]);
                  fprintf(out, ",");
                  ExprASTPrint(out, ast->child[1]);
            }
            else{
                  ExprASTPrint(out, ast->child[0]);
            }
            break;
      case assign:
                  ExprASTPrint(out, ast->child[0]);
                  fprintf(out, "=");
                  ExprASTPrint(out, ast->child[1]);
            break;
      case while_stmt:
                  fprintf(out,"while");
                  ExprASTPrint(out, ast->child[0]); 
                  fprintf(out, "\n");
                  ExprASTPrint(out, ast->child[1]);
            break;
      case if_stmt:
                  fprintf(out,"if");
                  ExprASTPrint(out, ast->child[0]); 
                  fprintf(out, "\n");
                  ExprASTPrint(out, ast->child[1]);
            break;
      case else_stmt:
                  fprintf(out,"else");
                  ExprASTPrint(out, ast->child[0]); 
                  fprintf(out, "\n");
                  ExprASTPrint(out, ast->child[1]);
            break;
      case ret_stmt:
                  fprintf(out,"return ");
                  fprintf(out,"(");
                  ExprASTPrint(out, ast->child[0]); 
                  fprintf(out,")");
            break;
      case print_stmt:
                  fprintf(out,"print ");
                  fprintf(out,"(");
                  ExprASTPrint(out, ast->child[0]); 
                  fprintf(out,")");
            break; 
      case funcall_stmt:
            fprintf(out,"function call");
            fprintf(out,"(");
            ExprASTPrint(out, ast->child[0]); 
            fprintf(out,")");
            break; 
      case funcall: 
            ExprASTPrint(out, ast->child[0]); 
            fprintf(out,"(");
            ExprASTPrint(out, ast->child[1]); 
            fprintf(out,")");
            break;
      case args:
            if(ast->child[0])
            ExprASTPrint(out, ast->child[0]); 
            break;
      case arglist:
            if(ast->child[1])
            {
                  ExprASTPrint(out, ast->child[0]); 
                  fprintf(out,",");
                  ExprASTPrint(out, ast->child[1]); 

            }
            else
                  ExprASTPrint(out, ast->child[0]); 
            break;
      case boolexpr:
                  fprintf(out,"(");
                  ExprASTPrint(out, ast->child[0]); 
                  fprintf(out,")");
            break;
      case nil:
            break;
   default:
         fprintf(out, "Unexpected construct! %s",ast_name[ast->type]);
         break;
   }
}


/*-----------------------------------------------------------------------
//
// Function: SExprASTPrint()
//
//   Print an AST representing a nanoLang construct as an
//   s-expression.
//
// Global Variables: -
//
// Side Effects    : Output
//
/----------------------------------------------------------------------*/

void SExprASTPrint(FILE* out, AST_p ast)
{
   int i;

   if(ast)
   {
      switch(ast->type)
      {
      case t_STRINGLIT:
            fprintf(out, "t_STRINGLIT<%s> ",
                    ast->litval);
            break;
      case t_IDENT:
            fprintf(out, "t_IDENT<%s> ",
                    ast->litval);
            break;
      case t_INTLIT:
            fprintf(out, "t_INTLIT<%s> ",
                    ast->litval);
            break;
      default:
            fprintf(out, "(");
            fprintf(out, "%s ", ast_name[ast->type]);
            for(i=0; ast->child[i]; i++)
            {
               SExprASTPrint(out, ast->child[i]);
            }
            fprintf(out, ") ");
            break;
      }
   }
}


/*---------------------------------------------------------------------*/
/*                        End of File                                  */
/*---------------------------------------------------------------------*/
