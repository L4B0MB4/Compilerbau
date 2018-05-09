
#include "types.h"
#include "ast.h"
#include "symbols.h"
#include "semantic.h"

void STBuildLibFunctions(SymbolTable_p st)
{
  STInsertSymbol(st,"StrCat", 3, 42, 42);
  STInsertSymbol(st,"IntToStr", 4, 42, 42);
  STInsertSymbol(st,"StrToInt", 5, 42, 42);
}


bool STBuildAllTables(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  bool res = true;
  int i;
  assert(st);

  if (ast)
  {
    ast->context = st;
    switch (ast->type)
    {
    case vardef:
      res = STBuildAllTables(st, tt, ast->child[0]) && res; // Typ der Var
      res = STBuildAllTables(st, tt, ast->child[1]) && res; //Name der Var
      res = STInsertVar(st, tt, ast) && res;                // hinzufügen der Variable in die Symboltabelle
      break;
    case fundef:
      res = STInsertFunDef(st, tt, ast) && res; // hinzufügen der Funktion in die Symboltabelle
      res = STBuildAllTables(st, tt, ast->child[0]) && res;
      res = STBuildAllTables(st, tt, ast->child[1]) && res;
      st = STEnterContext(st);
      res = st_insert_params(st, tt, ast->child[2]) && res; //hinzufügen der Parameter in die Symboltabelle
      res = STBuildAllTables(st, tt, ast->child[2]) && res;
      res = STBuildAllTables(st, tt, ast->child[3]) && res;
      st = STLeaveContext(st);
      break;
    case body:
      st = STEnterContext(st);
      ast->context = st;
      res = STBuildAllTables(st, tt, ast->child[0]) && res;
      res = STBuildAllTables(st, tt, ast->child[1]) && res;
      SymbolTablePrintLocal(stdout, st, tt);
      st = STLeaveContext(st);
      break;
    case stmts:
      check_statements(st, tt, ast);
    default:
      for (i = 0; ast->child[i]; i++)
      {
        res = STBuildAllTables(st, tt, ast->child[i]) && res;
      }
      break;
    }
  }
  return res;
}

int check_statements(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  int i = 0;
  for (i = 0; ast->child[i]; i++)
  {
    int currentType = ast->child[i]->type;
    if (currentType != body && currentType != nil && currentType != stmts)
    {
      if (currentType == assign)
      {
        int val = check_assign(st, tt, ast->child[i]);
        if (val == -1)
          printf("error: line %d %d -> there is something wrong with this assign\n", ast->yylineno, ast->yycol);
      }
      else if (currentType == funcall_stmt)
      {
        int ret = check_funcallstmt(st, tt, ast->child[i]->child[0]);
        if (ret == -1)
        {
          printf("error: line %d %d -> funcall statement is incorrect\n", ast->yylineno, ast->yycol);
        }
        else if (ret == -2)
        {
          printf("error: line %d %d -> couldnt find function \n", ast->yylineno, ast->yycol);
        }
      }
      else if (currentType == print_stmt)
      {
        int ttype = getType(st, tt, ast->child[i]->child[0]);
        if (ttype < 1)
        {
          printf("error: line %d %d -> There is something wrong with this print statement\n", ast->yylineno, ast->yycol);
        }
      }
      else if (currentType == ret_stmt)
      {
        //bonus
      }
      else // all other statements check
      {
        int val = check_compare(st, tt, ast->child[i]);
        if (val == -1)
          printf("error: line %d %d -> there is something wrong with this compare \n", ast->child[i]->child[0]->yylineno, ast->child[i]->child[0]->yycol);
      }
    }
  }
}

int check_compare(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  ast = ast->child[0];
  int retVal = getType(st, tt, ast->child[0]) == getType(st, tt, ast->child[1]);
  if (!retVal)
    return -1;
  else
    return retVal;
}

int check_funcallstmt(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  Symbol_p s = getSymbol(st, ast->child[0]);
  if (s)
  {
    NanoTypeCell nt;
    nt.constructor = tc_function;
    nt.typeargno = 1;
    nt.typeargs[0] = getType(st, tt, ast->child[0]);
    if (ast->child[1] && ast->child[1]->type != nil)
    {
      ast = ast->child[1];
      goThroughArgList(st, tt, ast, &nt);
      ++nt.typeargno;
      nt.typeargs[nt.typeargno - 1] = getType(st, tt, ast);
    }
    nt.typeargno--;
    if (TypeCmp(&nt, &tt->types[s->type]) == 0)
    {
      return 0;
    }
    else
      return -1;
  }
  else
    return -2;
}

void goThroughArgList(SymbolTable_p st, TypeTable_p tt, AST_p ast, NanoType_p nt)
{
  if (ast->child[0]->type == arglist)
  {
    goThroughArgList(st, tt, ast->child[0], nt);
    ++nt->typeargno;
    nt->typeargs[nt->typeargno - 1] = getType(st, tt, ast->child[1]);
  }
  else
  {
    ++nt->typeargno;
    nt->typeargs[nt->typeargno - 1] = getType(st, tt, ast->child[0]);
  }
}

int check_assign(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  int vartype1 = -2;
  int vartype2 = -2;
  if (ast->child[0]->child[0])
  {
    vartype1 = check_assign(st, tt, ast->child[0]);
  }
  else if (ast->child[0]->litval)
  {
    vartype1 = getType(st, tt, ast->child[0]);
  }
  if (ast->type == funcall)
  {
    return vartype1;
  }
  if (vartype1 > 0)
  {
    if (ast->child[1])
    {
      if (ast->child[1]->child[0])
      {
        vartype2 = check_assign(st, tt, ast->child[1]);
      }
      if (ast->child[1]->litval)
        vartype2 = getType(st, tt, ast->child[1]);
    }
    else // unariy minus 
    {
      return vartype1;
    }
    if (vartype1 == vartype2)
      return vartype1;
    else
      return -1;
  }
}

Symbol_p getSymbol(SymbolTable_p st, AST_p ast)
{
  if (ast->litval)
  {

    SymbolTable_p tmp = st;
    Symbol_p s = NULL;
    while (!s && tmp)
    {
      s = STFindSymbolLocal(tmp, ast->litval);
      tmp = tmp->context;
    }
    return s;
  }
  return NULL;
}

int getType(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  int retVal = -1;
  if (ast->type == funcall)
  {
    if (check_funcallstmt(st, tt, ast) == 0)
    {
      ast = ast->child[0];
    }
  }
  Symbol_p s = getSymbol(st, ast);
  if (s)
  {
    if (s->type > 2)
    {
      retVal = TypeTableGetRetType(tt, s->type);
      return retVal;
    }
  }
  if (s)
    return s->type;
  if (ast->type)
  {
    if (ast->type == t_INTLIT)
      retVal = 2;
    else if (ast->type == t_STRINGLIT)
      retVal = 1;
  }
  return retVal;
}

bool st_insert_params(SymbolTable_p st, TypeTable_p tt, AST_p past)
{
  for (int i = 0; past->child[i]; i++)
  {
    if (past->child[i]->type == paramlist)
    {
      st_insert_params(st, tt, past->child[i]);
      if (past->child[i + 1])
      {
        i++;
      }
    }
    if (past->child[i]->type == param)
    {
      switch (past->child[i]->child[0]->type)
      {
      case t_INTEGER:
        STInsertSymbol(st, past->child[i]->child[1]->litval, 2, 42, 42);
        break;
      case t_STRING:
        STInsertSymbol(st, past->child[i]->child[1]->litval, 1, 42, 42);
        break;
      default:
        printf("\n\n\n hoooooo \n\n\n");
        break;
      }
    }
  }
  return true;
}

bool STInsertVar(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  NanoTypeCell type;
  type.constructor = tc_atomic;
  int ttype;
  if (strcmp(ast->child[0]->litval, "Integer") == 0)
  {
    ttype = 2;
  }
  else if (strcmp(ast->child[0]->litval, "String") == 0)
  {
    ttype = 1;
  }
  else
  {
    ttype = 0;
  }
  if (ast->child[1]->child[0])
  {
    int i = 0;
    insertIdList(ttype, st, tt, ast->child[1]);
  }
  else
  {
    STInsertSymbol(st, ast->child[1]->litval, ttype, 42, 42);
  }
  return true;
}

void insertIdList(int ttype, SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  if (ast->child[0]->type == idlist)
  {
    insertIdList(ttype, st, tt, ast->child[0]);
    STInsertSymbol(st, ast->child[1]->litval, ttype, 42, 42);
  }
  else
  {
    STInsertSymbol(st, ast->child[0]->litval, ttype, 42, 42);
    STInsertSymbol(st, ast->child[1]->litval, ttype, 42, 42);
  }
}

bool insertParamsFunDef(SymbolTable_p st, TypeTable_p tt, AST_p past, NanoType_p nt)
{
  //recursive parameters -> nt

  for (int i = 0; past->child[i]; i++)
  {
    if (past->child[i]->type == paramlist)
    {
      insertParamsFunDef(st, tt, past->child[i], nt);
      if (past->child[i + 1])
        i++;
    }
    if (past->child[i]->type == param)
    {
      switch (past->child[i]->child[0]->type)
      {
      case t_INTEGER:
        nt->typeargs[nt->typeargno] = T_Integer;
        break;
      case t_STRING:
        nt->typeargs[nt->typeargno] = T_String;
        break;
      default:
        printf("\n\n\n haaaaaaa \n\n\n");
        break;
      }
      nt->typeargno++;
    }
  }
  return true;
}

bool STInsertFunDef(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  NanoTypeCell type;
  int val = 0;
  if (strcmp(ast->child[0]->litval, "Integer") == 0)
  {
    type.typeargs[0] = 2;
  }
  else if (strcmp(ast->child[0]->litval, "String") == 0)
  {
    type.typeargs[0] = 1;
  }
  else
  {
    type.typeargs[0] = 0;
  }
  int i = 0;
  type.typeargno = 1;

  AST_p past = ast->child[2];
  insertParamsFunDef(st, tt, past, &type);

  type.constructor = tc_function;
  if (type.typeargno == 1)
  {
    type.typeargs[type.typeargno] = 0;
  }
  int no = TypeTableGetTypeIndex(tt, &type);
  STInsertSymbol(st, ast->child[1]->litval, no, 42, 42);
  return true;
}