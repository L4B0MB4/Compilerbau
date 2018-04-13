
#include "symbols.h"
#include "types.h"

int main(void)
{
   TypeTable_p   tt;
   SymbolTable_p st;
   NanoTypeCell  type;

   printf("Creating type table...\n");
   tt = TypeTableAlloc();
   printf("Initial type table:\n");
   TypeTablePrint(stdout, tt);
   type.constructor = tc_function;
   type.typeargno = 3;
   type.typeargs[0] = T_String;
   type.typeargs[1] = T_String;
   type.typeargs[2] = T_Integer;
   TypeTableGetTypeIndex(tt, &type);
   printf("Second type table:\n");
   TypeTablePrint(stdout, tt);
   type.constructor = tc_function;
   type.typeargno = 4;
   type.typeargs[0] = T_Integer;
   type.typeargs[1] = T_String;
   type.typeargs[2] = T_Integer;
   type.typeargs[3] = T_String;
   TypeTableGetTypeIndex(tt, &type);
   printf("Third type table:\n");
   TypeTablePrint(stdout, tt);
   TypeTableGetTypeIndex(tt, &type);
   printf("Fourth type table (=3rd type table):\n");
   TypeTablePrint(stdout, tt);

   printf("\nBasic (empty) symbol table:\n");
   st = SymbolTableAlloc();
   SymbolTablePrintGlobal(stdout, st, tt);
   STInsertSymbol(st, "sym1", 1, 42, 42);
   STInsertSymbol(st, "sym2", 4, 42, 42); // ??? es gibt keinen 4. atomic type ? 
   st = STEnterContext(st);
   STInsertSymbol(st, "sym3", 2, 42, 42);
   printf("\nFilled symbol tables:\n");
   SymbolTablePrintGlobal(stdout, st, tt);

   return EXIT_SUCCESS;
}