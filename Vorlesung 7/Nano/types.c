

#include "types.h"


char* possible_atomic_types[] =
{
   "NoType",
   "String",
   "Integer"
};

// vergleicht Typen
int TypeCmp(NanoType_p t1, NanoType_p t2)
{
   int res, i;

   res = t1->constructor - t2->constructor;
   if(res)
   {
      return res;
   }
   res = t1->typeargno - t2->typeargno;
   if(res)
   {
      return res;
   }
   for(i=0; i<t1->typeargno; i++)
   {
      res = t1->typeargs[i] - t2->typeargs[i];
      if(res)
      {
         break;
      }
   }
   return res;
}


//printet types
void TypePrint(FILE* out, TypeTable_p table, TypeIndex type)
{
   int i;
   char* sep = "";

   switch(table->types[type].constructor)
   {
   case tc_atomic:
         fprintf(out, "%s", possible_atomic_types[type]);
         break;
   case tc_function:
         //printf("typeprint %d \n",table->types[type].typeargno);
         fprintf(out, "(");
         for(i=1; i < table->types[type].typeargno; i++)
         {
            fprintf(out, "%s", sep);
            TypePrint(out, table, table->types[type].typeargs[i]);
            sep = ", ";
         }
         fprintf(out, ") -> ");
         TypePrint(out, table, table->types[type].typeargs[0]);
         break;
   default:
         printf("Oups unkown constructor!\n");
         break;
   }
}


// Fügt die atomaren Typen zur TypeTable hinzu (Integer,String und NoType)
void TypeTableInit(TypeTable_p table)
{
   NanoTypeCell type;

   table->type_ctr=0;

   type.constructor = tc_atomic;
   type.typeargno   = 1;
   type.typeargs[0] = T_NoType;
   TypeTableInsert(table, &type);
   type.typeargs[0] = T_String;
   TypeTableInsert(table, &type);

   type.typeargs[0] = T_Integer;
   TypeTableInsert(table, &type);
}


// Neue TypeTable allokieren
TypeTable_p  TypeTableAlloc()
{
   TypeTable_p handle;

   handle = TypeTableCellAlloc();
   TypeTableInit(handle);

   return handle;
}


void TypeTableFree(TypeTable_p junk)
{
   TypeTableCellFree(junk);
}

//Darstellung eines Types durch Zahlnwert
void TypeTablePrint(FILE* out, TypeTable_p table)
{
   int i;

   for(i=0; i<table->type_ctr; i++)
   {
      fprintf(out, "%d: ", i);
      TypePrint(out, table, i);
      fprintf(out, "\n");
   }
}

//Einfügen eienes neuen Types in die TypeTable
TypeIndex TypeTableInsert(TypeTable_p table, NanoType_p type)
{
   if(table->type_ctr==MAXTYPES)
   {
      fprintf(stderr, "Tooooooooo many types >.< !\n");
      exit(EXIT_FAILURE);
   }
   table->types[table->type_ctr] = *type;
   table->type_ctr++;

   return table->type_ctr-1;
}


TypeIndex TypeTableGetTypeIndex(TypeTable_p table, NanoType_p type)
{
   int i;
   for(i=0; i<table->type_ctr; i++)
   {
      if(TypeCmp(type, &(table->types[i])) == 0)
      {
         return i;
      }
   }
   return TypeTableInsert(table, type);
}


TypeIndex TypeTableGetRetType(TypeTable_p table, TypeIndex type)
{

   return TypeRetType(&(table->types[type]));
}