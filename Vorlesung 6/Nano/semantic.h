bool STBuildAllTables(SymbolTable_p st, TypeTable_p tt, AST_p ast);
bool STInsertFunDef(SymbolTable_p st, TypeTable_p tt, AST_p ast);
bool STInsertVar(SymbolTable_p st, TypeTable_p tt, AST_p ast);
bool st_insert_params(SymbolTable_p st, TypeTable_p tt, AST_p ast);
bool check_assign(SymbolTable_p st,AST_p ast);
bool getExprType(SymbolTable_p st, AST_p ast, int hasToBeType);
Symbol_p getSymbol(SymbolTable_p st, char* symbol);
