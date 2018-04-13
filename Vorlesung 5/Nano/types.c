#include "types.h"
#include <stdio.h>

char* typenames[] =
{
   "NoType",
   "String",
   "Integer"
};

NanoType_p allocNanoType(void);

TypeTable_p TypeTableAlloc()
{
    TypeTable_p t =TypeTableCellAlloc();
    return t;
}


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

NanoType_p allocNanoType(void)
{
    return (NanoTypeCell*)malloc(sizeof(NanoTypeCell));
}

int TypeCmp(NanoType_p t1, NanoType_p t2)
{
    if(t1->constructor == t2->constructor &&
        t1->typeargno == t2->typeargno
    )
    {
        for(int i=0;i<t1->typeargno;i++)
        {
            if(t1->typeargs[i]!=t2->typeargs[i])
            {
                return 0;
            }
        }
        return 1;
    }
    else
        return 0;
}


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

void TypePrint(FILE* out, TypeTable_p table, TypeIndex type)
{
   int i;
   char* sep = "";

   switch(table->types[type].constructor)
   {
   case tc_atomic:
         fprintf(out, " atomic ! %s", typenames[type]);
         break;
   case tc_function:
         fprintf(out, "function !(");
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
         printf("Error, unknown type constructor!\n");
         break;
   }
}


TypeIndex TypeTableInsert(TypeTable_p table, NanoType_p type)
{
   if(table->type_ctr==MAXTYPES)
   {
      fprintf(stderr, "Type table overflow: Out of types!\n");
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
   assert((type>0) && (type<table->type_ctr));

   return TypeRetType(&(table->types[type]));
}
