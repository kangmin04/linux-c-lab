
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(void){
	pid_t pid = fork(); 
	if(pid == 0){
		_exit(42); 
	}
	
	int status; 
	waitpid(pid, &status, 0); 
	
	if(WIFEXITED(status)){
		printf("child process exit code : %d\n", WEXITSTATUS(status)); 
	}	
	return 0; 
}
