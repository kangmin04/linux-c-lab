#include<stdio.h>
#include<signal.h>
#include<unistd.h> 

int main(void){
	printf("send SIGTERM to myself. \n"); 
	raise(SIGTERM); 

	printf("CANT NOT REACH TO HERE"); 
	return 0; 
}
