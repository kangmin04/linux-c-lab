/* when fork occurs, parent's fd is duplicated to child. (which fd number indicate certain file. And that certain files are shared by parent and child.)
 *
 * pipe() : to communicate between other process, using IPC. 
 * when pipe(int pipefd[2]) invoke -> pipefd[0] assigned to read, and pipefd[1] assigend to write
 *
 * So. invoke pipe first and then invoke fork(), so that fd is duplicated and parent, child has each others file. which means, parent and child have pipefd[0], pipefd[2] to each other. THAT'S HOW YOU IPC 
 * */


#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

int main(void){
	int fd[2]; 
	pipe(fd);  //fd[0] : read end, fd[1] : write end
	
       pid_t pid = fork(); 

	if(pid == 0) { //child -> only write, and just close read one. 
		close(fd[0]); 
		char *msg = "MESSAGE FROM CHILD MOMMY\n";
		write(fd[1], msg, strlen(msg)); 
		close(fd[1]);
	        _exit(0); 	
		
	}else{
		close(fd[1]); // parent do only read -> remove(close) write
		char buf[128] = {0}; 
		read(fd[0], buf, sizeof(buf)-1); 
		printf("MSG FROM PARENT: %s", buf); 
		close(fd[0]); 
		wait(NULL);  // parent must call(invoke) wait, waitpid!! if not, when child done its executing first without wait, child process would be zombie process and we can't kill it 
	}	

	return 0; 
}




