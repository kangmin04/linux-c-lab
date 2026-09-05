#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h> 

int main(void){
	pid_t pid = fork(); 
	printf("difference between pid, getpid() : %d, %d\n", pid, getpid()); 
	printf("---------------------\n");

	if(pid < 0){
		perror("fork failed"); 
		return 1; 
	}else if(pid == 0){
		printf("[child %d] instantly exit(7)\n", getpid()); 
		_exit(7); 
	} else{
		printf("[parent] child(%d) just shut down. ", pid);
	        printf("use cli : ps aux | grep %d\n" , pid); 

		sleep(10) ; // REMAIN child process in zombie for 10 sec
		
		int status; 
		pid_t reaped = wait(&status); // wait : find the latest zombie process and release it from memory. 
					     // we give uninitialized variable's address to track its exit code and etc, 
		if(WIFEXITED(status)){
			printf("[Parent] PID %d release. exit code : %d\n", reaped, WEXITSTATUS(status)); 
		}

	}
	
	return 0; 

}
