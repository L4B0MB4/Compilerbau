#include "compile.h"

char *atomic_type_possibilities[] =
    {
        "void",
        "char*",
        "int"};

TypeTable_p tt;
FILE *out;
int bodyCounter = 0;

void compileMainMethod(SymbolTable_p st, TypeTable_p tt)
{
    fprintf(out, "int main(char argc, char** argv){ N_main();}");
}

void compileSetup(FILE *oout)
{
    out = oout;
}

void compileDeclarations(SymbolTable_p st, TypeTable_p table)
{
    fprintf(out, "#include <stdio.h>\n");
    if (!tt)
        tt = table;
    int i;
    for (i = 0; i < st->symbol_ctr; i++)
    {
        compileDeclarationType(tt, st->symbols[i].type, st->symbols[i].symbol);
    }
    fprintf(out, "\n\n\n");
}

void compileDeclarationType(TypeTable_p table, TypeIndex type, char *symbol)
{
    int i;
    char *sep = "";

    switch (table->types[type].constructor)
    {
    case tc_function:
        compileFunDefType(table->types[type].typeargs[0]);
        fprintf(out, " N_%s (", symbol);
        for (i = 1; i < table->types[type].typeargno; i++)
        {
            fprintf(out, "%s", sep);
            compileFunDefType(table->types[type].typeargs[i]);
            sep = ", ";
        }
        fprintf(out, ");");
        compileBreakAndTabs();
        break;
    default:
        fprintf(out, "Oups unkown constructor!\n");
        break;
    }
}

void compileFunDefType(TypeIndex t)
{
    switch (t)
    {
    case T_Integer:
        fprintf(out, "int");
        break;
    case T_String:
        fprintf(out, "char*");
        break;
    }
}

void compileRecursiveChildren(AST_p ast, TypeTable_p ttp)
{
    if (!tt)
    {
        tt = ttp;
    }
    int i = 0;
    for (i = 0; ast->child[i]; i++)
    {
        switching(ast->child[i]);
    }
}

void switching(AST_p ast)
{
    switch (ast->type)
    {
    case assign:
        compileAssign(ast);
        break;
    case while_stmt:
        compileCompare("while", ast);
    case if_stmt:
        compileCompare("if", ast);
        break;
    case body:
        fprintf(out, "{");
        ++bodyCounter;
        compileBreakAndTabs();
        compileRecursiveChildren(ast, tt);
        --bodyCounter;
        fprintf(out, "}");
        compileBreakAndTabs();
        break;
    case funcall:
        compileFunCall(ast);
        break;
    case t_INTLIT:
    case t_STRINGLIT:
        fprintf(out, "%s", ast->litval);
        break;
    case t_IDENT:
        fprintf(out, "N_%s", ast->litval);
        break;
    case funcall_stmt:
        compileFunCallStmt(ast);
        break;
    case t_INTEGER:
        fprintf(out, "int ");
        break;
    case t_STRING:
        fprintf(out, "char* ");
        break;
    case fundef:
        compileFundef(ast);
        break;
    case vardefs:
        compileVarDefs(ast);
        break;
    case print_stmt:
        compilePrintStmt(ast);
        break;
    case ret_stmt:
        fprintf(out, "return ");
        switching(ast->child[0]);
        fprintf(out, ";");
        compileBreakAndTabs();
        break;
    case t_PLUS:
    case t_MINUS:
    case t_MULT:
    case t_DIV:
        compileExpression(ast);
        break;
    default:
        compileRecursiveChildren(ast, tt);
        break;
    }
}

void compileExpression(AST_p ast)
{
    fprintf(out, "(");
    if (!ast->child[1])
    {
        fprintf(out, "-");
        switching(ast->child[0]);
        fprintf(out, ")");
        return;
    }
    switching(ast->child[0]);
    switch (ast->type)
    {
    case t_PLUS:
        fprintf(out, "+");
        break;
    case t_MINUS:
        fprintf(out, "-");
        break;
    case t_MULT:
        fprintf(out, "*");
        break;
    case t_DIV:
        fprintf(out, "/");
        break;
    }
    if (ast->child[1])
    {
        switching(ast->child[1]);
    }
    fprintf(out, ")");
}

void compilePrintStmt(AST_p ast)
{
    switch (ast->child[0]->type)
    {
    case t_INTLIT:
        fprintf(out, "printf(\"");
        fprintf(out, "%s", ast->child[0]->litval);
        fprintf(out, "\");");
        compileBreakAndTabs();
        fprintf(out, "printf(\"\\n\");");
        compileBreakAndTabs();
        return;
    case t_STRINGLIT:
        fprintf(out, "printf(");
        fprintf(out, " %s", ast->child[0]->litval);
        fprintf(out, ");");
        compileBreakAndTabs();
        fprintf(out, "printf(\"\\n\");");
        compileBreakAndTabs();
        return;
    }
    TypeIndex rettype;
    if (ast->child[0]->type == funcall)
    {
        rettype = STSymbolReturnType(ast->context, tt, ast->child[0]->child[0]->litval);
        fprintf(out, "printf(\"%%");
        switch (rettype)
        {
        case T_String:
            fprintf(out, "s");
            break;
        case T_Integer:
            fprintf(out, "d");
            break;
        }
        fprintf(out, "\", ");
        switching(ast->child[0]);
        fprintf(out, ");");
        compileBreakAndTabs();
        fprintf(out, "printf(\"\\n\");");
        compileBreakAndTabs();
        return;
    }
    rettype = STSymbolReturnType(ast->context, tt, ast->child[0]->litval);

    switch (rettype)
    {
    case T_Integer:
        fprintf(out, "printf(\"%%d\",");
        fprintf(out, "N_%s", ast->child[0]->litval);
        fprintf(out, ");");
        compileBreakAndTabs();
        fprintf(out, "printf(\"\\n\");");
        compileBreakAndTabs();
        break;
    case T_String:
        fprintf(out, "printf(\"%%s\",");
        fprintf(out, "N_%s", ast->child[0]->litval);
        fprintf(out, ");");
        compileBreakAndTabs();
        fprintf(out, "printf(\"\\n\");");
        compileBreakAndTabs();
    }
}

void compileAssign(AST_p ast)
{
    switching(ast->child[0]);
    fprintf(out, "= ");
    switching(ast->child[1]);
    fprintf(out, ";");
    compileBreakAndTabs();
}

void compileVarDefs(AST_p ast)
{
    if (ast->child[0])
    {
        if (ast->child[0]->type == vardefs)
        {
            compileVarDefs(ast->child[0]);
            switching(ast->child[1]->child[0]);
            if (ast->child[1]->child[1]->type == idlist)
            {
                compileIdList(ast->child[1]->child[1]);
            }
            else
            {
                switching(ast->child[1]->child[1]);
            }
            fprintf(out, ";");
            compileBreakAndTabs();
        }
        else if (ast->child[0]->type == nil)
        {
            ast = ast->child[1];
            switching(ast->child[0]);
            if (ast->child[1]->type == idlist)
            {
                compileIdList(ast->child[1]);
            }
            else
            {
                switching(ast->child[1]);
            }
            fprintf(out, ";");
            compileBreakAndTabs();
        }
    }
}

void compileIdList(AST_p ast)
{
    if (ast->child[0]->type == idlist)
    {
        compileIdList(ast->child[0]);
        fprintf(out, ", ");
        switching(ast->child[1]);
    }
    else
    {
        switching(ast->child[0]);
        fprintf(out, ", ");
        switching(ast->child[1]);
    }
}

void compileFundef(AST_p ast)
{
    switching(ast->child[0]);
    switching(ast->child[1]);
    fprintf(out, "(");
    compileParams(ast->child[2]);
    fprintf(out, ")");
    switching(ast->child[3]);
}

void compileParams(AST_p ast)
{
    if (ast->child[0])
    {

        if (ast->child[0]->type == paramlist)
        {
            compileParams(ast->child[0]);
            if (ast->child[1])
            {
                switching(ast->child[1]->child[0]);
                switching(ast->child[1]->child[1]);
                fprintf(out, ", ");
            }
        }
        else if (ast->child[0]->type == param && !ast->child[1])
        {
            switching(ast->child[0]->child[0]);
            switching(ast->child[0]->child[1]);
        }
        else
        {
            switching(ast->child[0]->child[0]);
            switching(ast->child[0]->child[1]);
            fprintf(out, ", ");
            switching(ast->child[1]->child[0]);
            switching(ast->child[1]->child[1]);
        }
    }
}

void compileRecursiveParent(AST_p ast)
{
    switching(ast);
    compileRecursiveChildren(ast, tt);
}

void compileFunCall(AST_p ast)
{
    fprintf(out, " N_%s(", ast->child[0]->litval);
    compileArglist(ast->child[1]);
    fprintf(out, ")");
}

void compileFunCallStmt(AST_p ast)
{
    compileFunCall(ast->child[0]);
    fprintf(out, ";");
    compileBreakAndTabs();
}

void compileArglist(AST_p ast)
{
    int ptype = ast->child[0]->type;
    if (ast->child[0]->type == arglist)
    {
        compileArglist(ast->child[0]);
        int type = ast->child[1]->type;
        if (type == t_INTLIT || type == t_STRINGLIT)
        {
            fprintf(out, ", %s", ast->child[1]->litval);
        }
        else
        {
            fprintf(out, ", %s", ast->child[1]->litval);
        }
    }
    else if (ptype == t_DIV || ptype == t_MINUS || ptype == t_PLUS || ptype == t_MULT)
    {
        switching(ast);
    }
    else
    {
        int type = ast->child[0]->type;
        if (type == t_INTLIT || type == t_STRINGLIT)
        {
            fprintf(out, "%s", ast->child[0]->litval);
        }
        else
        {
            fprintf(out, "%s", ast->child[0]->litval);
        }
    }
}

void compileCompare(char *val, AST_p ast)
{
    compileBreakAndTabs();
    fprintf(out, "%s", val);
    fprintf(out, "(");
    compileStringCompare(ast->child[0]->child[0]);
    switch (ast->child[0]->type)
    {

    case t_NEQ:
        fprintf(out, " != ");
        break;
    case t_GEQ:
        fprintf(out, " >= ");
        break;
    case t_GT:
        fprintf(out, " > ");
        break;
    case t_LEQ:
        fprintf(out, " <= ");
        break;
    case t_LT:
        fprintf(out, " < ");
        break;
    case t_EQ:
        fprintf(out, " == ");
        break;
    }
    compileStringCompare(ast->child[0]->child[1]);
    fprintf(out, ")");
    switching(ast->child[1]);
}

void compileStringCompare(AST_p ast)
{
    if (ast->type == t_STRINGLIT || ast->type == t_IDENT)
    {

        int type = T_String;
        if (ast->type == t_IDENT)
        {
            type = STFindSymbolGlobal(ast->context, ast->litval)->type;
        }
        if (type == T_String)
        {
            fprintf(out, "strlen(");
            switching(ast);
            fprintf(out, ")");
        }
        else
        {
            switching(ast);
        }
    }
    else
    {
        switching(ast);
    }
}

void compileBreakAndTabs()
{
    fprintf(out, "\n");
    int i = 0;
    for (i = 0; i < bodyCounter; i++)
    {
        fprintf(out, "\t");
    }
}