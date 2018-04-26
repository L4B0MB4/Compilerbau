
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
    case assign:
        printf("assign result %d\n",check_assign(st,tt,ast));
        break;
    default:
        for(i = 0;ast->child[i];i++)
        {
            res = STBuildAllTables(st,tt,ast->child[i]) && res;
        }
        break;
    }
  }
  return res;
}


int check_assign(SymbolTable_p st,TypeTable_p tt, AST_p ast)
{
  int vartype1 =-2;
  int vartype2 = -2;
  if(ast->child[0]->child[0])
  {
    vartype1=check_assign(st,tt,ast->child[0]);
  }
  else if (ast->child[0]->litval)
  {
    vartype1 = getType(st,tt,ast->child[0]);
      printf("vartype 1 %d\n",vartype1);
  }
  if(vartype1>0)
  {
    if(ast->child[1]->child[0])
    {
      vartype2 = check_assign(st,tt,ast->child[1]);
    }
    else if (ast->child[1]->litval)
    {
      vartype2= getType(st,tt,ast->child[1]);
      printf("vartype 2 %d\n",vartype2);
    }
    if(vartype2>0)
    {
      printf("vartype1 und 2 %d %d \n",vartype1,vartype2);
    }
    if(vartype1==vartype2)
      return vartype1;
    else 
      return -1;
  }
  /*
  SymbolTable_p tmp=st;
  printf("litval %s \n",ast->child[0]->litval);
  //es kann vorkommen ((i)+(i))+i) usw... immer unterstes element checken und hoch geben ... also vartype setzen?!
  //if(!ast->child[0]->litval)return check_assign(st,ast->child[0]); 
  int vartype=-2;
  
  if(ast->child[0]->child[0])
  {
    vartype = check_assign(st,ast->child[0]);
  }
  else
  {
      
    Symbol_p s =getSymbol(st,ast->child[0]->litval);
    if(!s)
    {
      if(ast->child[0]->type ==t_INTLIT)vartype =2;
      else if(ast->child[0]->type==t_STRINGLIT)vartype=1;
      if(!vartype)
      {
        printf("%s not defined !!!",ast->child[0]->litval);
        return false;
      }
    }
    if(!vartype)vartype = s->type;
  }
  printf("vartype: %d \n",vartype);

  if(ast->child[1]->child[0])
  {
    if(check_assign(st,ast->child[1]) == vartype)
    {
      return vartype;
    }
      return -1;
  }
  else if(ast->child[0]->child[0])
  {
    if(check_assign(st,ast->child[0]) == vartype)
    {
      return vartype;
    }
      return -1;
  }
  else
  {
    Symbol_p sp =getSymbol(st,ast->child[1]->litval);
    bool l =false;
    if(vartype==2)
    {
        l =  ast->child[1]->type == t_INTLIT;
    }
    else if(vartype==1)
    {
        l =  ast->child[1]->type == t_STRINGLIT ;
    }
    else
    {
      printf("NO TYPE \n");
      return false;
    }
    if(sp && !l)
    {
      l = sp->type == vartype;
    }
    if(l)
    {
      return vartype;
    }
    else
      return -1;
  }*/
}

int getType(SymbolTable_p st, TypeTable_p tt, AST_p ast)
{
  int retVal =-1;
  if(ast->litval)
  {

    SymbolTable_p tmp=st;
    Symbol_p s=NULL;
    while(!s&&tmp)
    {
      s =STFindSymbolLocal(tmp,ast->litval);
      tmp = tmp->context;
    }
    if(s)
    {
      if(s->type>2)
      {
        retVal = TypeTableGetRetType(tt,s->type);
        printf("tyyyyype %s\n",ast->litval);
        printf("tyyyyype %d\n",retVal);
        return retVal;
      }
    }
    if(s)return s->type;
  }
  if(ast->type)
  {
    if(ast->type==t_INTLIT)retVal =2;
    else if(ast->type==t_STRINGLIT) retVal =1;
  }
  return retVal;
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
  int ttype;
   if(strcmp(ast->child[0]->litval,"Integer")==0)
  {
    ttype = 2;
  }
  else if(strcmp(ast->child[0]->litval,"String")==0)
  {
    ttype=1;
  }
  else
  {
    ttype =0;
  }
  if(ast->child[1]->child[0])
  {
    int i=0;
    for(i=0;ast->child[1]->child[i];i++)
    {
      STInsertSymbol(st, ast->child[1]->child[i]->litval, ttype, 42, 42);
    }
  }
  else
  {
    STInsertSymbol(st, ast->child[1]->litval, ttype, 42, 42);
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