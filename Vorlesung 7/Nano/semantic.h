bool STBuildAllTables(SymbolTable_p st, TypeTable_p tt, AST_p ast);
bool STInsertFunDef(SymbolTable_p st, TypeTable_p tt, AST_p ast);
bool STInsertVar(SymbolTable_p st, TypeTable_p tt, AST_p ast);
bool st_insert_params(SymbolTable_p st, TypeTable_p tt, AST_p ast);
int check_assign(SymbolTable_p st, TypeTable_p tt, AST_p ast);
int getType(SymbolTable_p st, TypeTable_p tt, AST_p ast);
int check_funcallstmt(SymbolTable_p st,TypeTable_p tt, AST_p ast);
Symbol_p getSymbol(SymbolTable_p st, AST_p ast);
void goThroughArgList(SymbolTable_p st, TypeTable_p tt, AST_p ast, NanoType_p nt);
bool insertParamsFunDef(SymbolTable_p st,TypeTable_p tt, AST_p past, NanoType_p nt);
int check_statements(SymbolTable_p st, TypeTable_p tt, AST_p ast);
int check_compare(SymbolTable_p st, TypeTable_p tt, AST_p ast);
void insertIdList(int ttype, SymbolTable_p st, TypeTable_p tt,AST_p ast);


void STBuildLibFunctions(SymbolTable_p st);
