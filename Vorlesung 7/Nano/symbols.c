
#include "symbols.h"
#include "types.h"



SymbolTable_p SymbolTableAlloc(void)
{
   SymbolTable_p handle = SymbolTableCellAlloc();

   handle->context    = NULL;
   handle->symbol_ctr = 0;

   return handle;
}


void SymbolTableFree(SymbolTable_p junk)
{
   int i;

   if(junk)
   {
      for(i=0; i< junk->symbol_ctr; i++)
      {
         free(junk->symbols[i].symbol);
      }
   }
   SymbolTableCellFree(junk);
}

//in den nächst höheren Scope
SymbolTable_p STEnterContext(SymbolTable_p table)
{
   SymbolTable_p handle = SymbolTableAlloc();

   handle->context = table;

   return handle;
}


SymbolTable_p STLeaveContext(SymbolTable_p table)
{
   return table->context;
}


// Finde Typ in lokaler Symboltabelle
Symbol_p STFindSymbolLocal(SymbolTable_p table, char* symbol)
{
   int i;

   for(i=0; i< table->symbol_ctr; i++)
   {
      if(strcmp(table->symbols[i].symbol, symbol) == 0)
      {
         return &(table->symbols[i]);
      }
   }
   return NULL;
}


Symbol_p STFindSymbolGlobal(SymbolTable_p table, char* symbol)
{
   Symbol_p res;

   if(!table)
   {
      return NULL;
   }
   res = STFindSymbolLocal(table, symbol);

   if(!res)
   {
      res = STFindSymbolGlobal(table->context, symbol);
   }
   return res;
}

// return type einer funktion
TypeIndex STSymbolReturnType(SymbolTable_p table, TypeTable_p tt, char* symbol)
{
   Symbol_p entry = STFindSymbolGlobal(table, symbol);

   if(!entry)
   {
      return T_NoType;
   }
   return TypeTableGetRetType(tt, entry->type);
}


bool STInsertSymbol(SymbolTable_p table, char* symbol, TypeIndex type,
                    int line, int col)
{
   Symbol_p handle;

   handle = STFindSymbolLocal(table, symbol);
   if(handle)
   {
      printf("%d:%d: Oups: symbol '%s' doubly defined )\n",
             line, col, symbol);
      return false;
   }
   if(table->symbol_ctr==MAXSYMBOLS)
   {
      fprintf(stderr, "Waaaay to many symbols in context :P !\n");
      exit(EXIT_FAILURE);
   }
   table->symbols[table->symbol_ctr].symbol = strdup(symbol);
   table->symbols[table->symbol_ctr].line   = line;
   table->symbols[table->symbol_ctr].col    = col;
   table->symbols[table->symbol_ctr].type   = type;

   table->symbol_ctr++;

   return true;
}

void SymbolTablePrintLocal(FILE* out, SymbolTable_p st, TypeTable_p tt)
{
   int i;

   for(i=0; i< st->symbol_ctr; i++)
   {
      fprintf(out, "%-10s: ", st->symbols[i].symbol);
      TypePrint(out, tt, st->symbols[i].type);
      fprintf(out, "\n");
   }
}


void SymbolTablePrintGlobal(FILE* out, SymbolTable_p st, TypeTable_p tt)
{
   if(st)
   {
      fprintf(out, "-----------New Recursive Print Layer------------\n");
      SymbolTablePrintLocal(out, st, tt);
      SymbolTablePrintGlobal(out, st->context, tt);
   }
}


