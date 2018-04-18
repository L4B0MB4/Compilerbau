
   #include "types.h"
   #include "ast.h"
   #include "symbols.h"
   #include "semantic.h"

bool STBuildAllTables(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  bool res = true;
  int i;

  assert(st);

  if(ast)
  {
    ast -> context = st;
    switch(ast -> type)
    {
      case vardef:
        printf("vardef\n");
        res = STBuildAllTables(st, tt, ast -> child[0]) && res;
        res = STBuildAllTables(st, tt, ast -> child[1]) && res;
        res = STInsertVar(st, tt, ast) && res;
        break;
      case fundef:
        printf("fundef\n");
        res = STInsertFunDef(st, tt, ast) && res;
        res = STBuildAllTables(st, tt, ast -> child[0]) && res;
        res = STBuildAllTables(st, tt, ast -> child[1]) && res;
        st = STEnterContext(st);
        res = st_insert_params(st, tt, ast -> child[2]) && res;
        res = STBuildAllTables(st, tt, ast -> child[2]) && res;
        res = STBuildAllTables(st, tt, ast -> child[3]) && res;
        st = STLeaveContext(st);
        break;
    case body: 
        printf("body\n");
        st= STEnterContext(st);
        ast->context =st;
        res = STBuildAllTables(st,tt,ast->child[0]) &&res;
        res = STBuildAllTables(st,tt,ast->child[1]) &&res;
        SymbolTablePrintLocal(stdout,st,tt);
        st = STLeaveContext(st);
        break;
    default:
        for(i = 0;i<ast->child[i];i++)
        {
            res = STBuildAllTables(st,tt,ast->child[i]) && res;
        }
        break;
    }
  }
  return res;
}


bool st_insert_params(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  NanoTypeCell  type;
  AST_p past = ast;
   for(int i=0;i<2;i++)
   {
     if(past->child[i])
     {
       if(past->child[i]->type==paramlist)
       {
         past = past->child[i];
         i=-1;
         continue;
       }
       if(past->child[i]->type==param)
       {
         switch(past->child[i]->child[0]->type)
         {
           case t_INTEGER:
              STInsertSymbol(st,past->child[i]->child[1]->litval, 2, 42, 42);
              break;
           case t_STRING:
              STInsertSymbol(st,past->child[i]->child[1]->litval, 1, 42, 42);
              break;
            default:
              printf("\n\n\n hoooooo \n\n\n");
              break;
         }
       }
     }else
        break;
   }
  return true;
}

bool STInsertVar(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  NanoTypeCell  type;
  type.constructor =tc_atomic;
   if(strcmp(ast->child[0]->litval,"Integer")==0)
  {
    STInsertSymbol(st, ast->child[1]->litval, 2, 42, 42);
  }
  else if(strcmp(ast->child[0]->litval,"String")==0)
  {
    STInsertSymbol(st, ast->child[1]->litval, 1, 42, 42);
  }
  else
  {
    STInsertSymbol(st, ast->child[1]->litval, 0, 42, 42);
  }
  return true;
}


bool STInsertFunDef(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  NanoTypeCell  type;
  int val =0;
  if(strcmp(ast->child[0]->litval,"Integer")==0)
  {
    type.typeargs[0]=2;
  }
  else if(strcmp(ast->child[0]->litval,"String")==0)
  {
    type.typeargs[0]=1;
  }
  else
  {
    type.typeargs[0]=0;
  }
  int i =0; 
  int a=1;

   AST_p past = ast->child[2];
   for(int i=0;i<2;i++)
   {
     if(past->child[i])
     {
       if(past->child[i]->type==paramlist)
       {
         past = past->child[i];
         i=-1;
         continue;
       }
       if(past->child[i]->type==param)
       {
         switch(past->child[i]->child[0]->type)
         {
           case t_INTEGER:
            type.typeargs[a]=T_Integer;
              break;
           case t_STRING:
            type.typeargs[a]=T_String;
              break;
            default:
              printf("\n\n\n haaaaaaa \n\n\n");
              break;
         }
         a++;
       }
     }else
        break;
   }
   type.constructor = tc_function;
   if(a==1)
   {
     type.typeargs[a]=0;
   }
   type.typeargno = a;
   int no = TypeTableGetTypeIndex(tt, &type);
   STInsertSymbol(st, ast->child[1]->litval, no, 42, 42);
  return true;
}