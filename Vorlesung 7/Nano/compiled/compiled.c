#include <stdio.h>
int N_test (int, int);
char* N_main ();



int N_test(int N_x, int N_y){
return (N_x+N_y);
}
char* N_main(){
int N_x;
int N_i, N_j;
N_i= 1;
printf( "i");
printf("\n");printf("%d",N_i);
printf("\n");printf("%d",  N_test(1, 2));
printf("\n");
if(N_j== N_test(1, 2)){
 N_test(10, 12);
}

if(N_j<N_i){
N_i= (((N_i*2)+(N_i*3))+ N_test(10, 12));
}
printf("%d",N_i);
printf("\n");printf( "Hallo miteinander");
printf("\n");return "hallo";
}
int main(char argc, char** argv){ N_main();}