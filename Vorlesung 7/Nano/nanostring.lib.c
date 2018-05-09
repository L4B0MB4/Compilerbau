#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/*char* N_StrCat(char* str1, char* str2);
int N_StrToInt(char* str);
char* N_IntToStr(int i);*/

char* N_StrCat(char* str1, char* str2)
{
      char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
      strcpy(str3, str1);
      strcat(str3, str2);
    return str3;
}
int N_StrToInt(char* str)
{
    if(strlen(str)<1) return 0;
    return atoi(str);
}
char* N_IntToStr(int i)
{
    char *ptr = (char *) malloc(12 * sizeof (char));
    sprintf(ptr,"%d",i);
    return ptr;
}

