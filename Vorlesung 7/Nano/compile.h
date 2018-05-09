#include "ast.h"
#include <stdio.h>
void compileSetup(FILE* oout);
void compileMainMethod(SymbolTable_p st, TypeTable_p tt);
void compileDeclarations(SymbolTable_p st, TypeTable_p table);
void compileDeclarationType(TypeTable_p table, TypeIndex type, char* symbol);
void compileFunDefType(TypeIndex t);
void compileRecursiveChildren(AST_p, TypeTable_p);
void compileFunCall(AST_p);
void compileArglist(AST_p);
void compileCompare(char*,AST_p);
void compileRecursiveParent(AST_p);
void switching(AST_p);
void compileFunCallStmt(AST_p ast);
void compileParams(AST_p ast);
void compileFundef(AST_p ast);
void compileVarDefs(AST_p ast);
void compileIdList(AST_p ast);
void compileAssign(AST_p ast);
void compilePrintStmt(AST_p ast);
void compileExpression(AST_p ast);
void compileStringCompare(AST_p ast);
void compileBreakAndTabs();
