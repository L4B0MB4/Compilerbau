#include <stdio.h>
#include <stdlib.h>

#define MAXMEM 1024*1024

int mem[MAXMEM];
int R0,R1,R2,R3, R4;


/* User code goes here - use only minimal C */

void user_code(void)
{
    loop:
        if(R0 ==R4)
            goto end;
        R1 = R1 * R0;
        R0--;
        goto loop;
    end:
    return;
}


/* Initialization and start-up can use full C */

int main(int argc, char* argv[])
{
   R0 = 0;
   R1 = 0;
   R2 = 0;
   R3 = 0;
   R4 = 0;
   for(int i = 0; i<MAXMEM; i++)
   {
      mem[i] = 0;
   }
   if(argc >1)
   {
      R0 = atoi(argv[1]);
   }
   if(argc > 2)
   {
      R1 = atoi(argv[2]);
   }
   if(argc >3)
   {
      R2 = atoi(argv[3]);
   }
   if(argc > 4)
   {
      R3 = atoi(argv[4]);
   }
   if(argc > 5)
   {
      R4 = atoi(argv[5]);
   }

   printf("Start\nR0: %d\nR1: %d\nR2: %d\nR3: %d\nR4: %d\n",
          R0,R1,R2,R3,R4);

   user_code();

   printf("\nEnd\nR0: %d\nR1: %d\nR2: %d\nR3: %d\nR4: %d\n",
          R0,R1,R2,R3,R4);
   return 0;
}
