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
    return atoi(str);
}
char* N_IntToStr(int i)
{
    char *ptr = (char *) malloc(12 * sizeof (char));
    sprintf(ptr,"%d",i);
    return ptr;
}int N_Euklid (int, int);
int N_main ();



int N_Euklid(int N_a, int N_b){
	
	while(N_a != N_b){
		
		if(N_a > N_b){
			N_a= (N_a-(-(-N_b)));
			}
		
		else{
			N_b= (N_b-N_a);
			}
		}
	
	if(N_a != N_b){
		
		if(N_a > N_b){
			N_a= (N_a-(-(-N_b)));
			}
		
		else{
			N_b= (N_b-N_a);
			}
		}
	return N_a;
	}
int N_main(){
	printf("%s",  N_StrCat("hallo", "\n"));
	printf("\n");
	return 0;
	}
int main(char argc, char** argv){ N_main();}