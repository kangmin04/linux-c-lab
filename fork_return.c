#include<stdio.h>
#include<unistd.h> 

int main(void){
	pid_t pid = fork();

	if(pid < 0){
		perror("fork failed");
	}else if (pid == 0){
		printf("child"); 
	}else {
		printf("parent %d", pid);
	}
	return 0; 
}
