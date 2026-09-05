#include<fcntl.h> // OPEN, O_CREAT import 
#include<unistd.h> // for OS systemcall like write, close 
#include<string.h>

int main(void){
	int fd = open("greeting.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); // O_WRONLY : write only, 0644 : access permission when  O_CREAT happens rwx
	if(fd == -1){
		return 1; 
	}

	const char *msg = "Hello, low-level fd! \n" ; 
	write(fd, msg, strlen(msg));
	close(fd); 

	// fd === 1. it's same integer
	write(1, "this line is write directly to stdout by using fd 1\n", 42); 
	return 0;

}
