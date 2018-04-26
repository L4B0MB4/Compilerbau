bool STBuildAllTables(SymbolTable_p st, TypeTable_p tt, AST_p ast);
bool STInsertFunDef(SymbolTable_p st, TypeTable_p tt, AST_p ast);
bool STInsertVar(SymbolTable_p st, TypeTable_p tt, AST_p ast);
bool st_insert_params(SymbolTable_p st, TypeTable_p tt, AST_p ast);
int check_assign(SymbolTable_p st, TypeTable_p tt, AST_p ast);
int getType(SymbolTable_p st, TypeTable_p tt, AST_p ast);
