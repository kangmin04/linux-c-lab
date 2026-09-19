#include<fcntl.h> // file 생성 플래그 (O_CREAT, .. )
#include<unistd.h> // 시스템콜(read, open, write.. ) 
#include<stdio.h> 
#include<stdlib.h>

#define BUFFERSIZE 4096 //4kb
#define COPYMODE 0644

void oops(char* , char* ); 

int main(int argc, char* argv[]){

	char buf[BUFFERSIZE]; 
	int n_chars; 

	if(argc != 3)	oops("invalid arguemnts. try it again. cp | srcfile | destfile", argv[0]); 


	int input_fd = open(argv[1], O_RDONLY); 
	if(input_fd == -1) oops("input error", argv[1]); 

	int output_fd = creat(argv[2], 0644);  // open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)
	if(output_fd == -1) oops("output file create failed.", argv[2]); 
	
	// read: 성공 시 실제 읽어들인 바이트 수 / EOF 시 0 / 실패시 -1반환 후 errno 설정 
	while((n_chars = read(input_fd, buf, BUFFERSIZE)) > 0 ){
		if(write(output_fd, buf, n_chars) != n_chars){
			oops("write error to ", argv[2]); 
		}
	}
	if(n_chars == -1) {
		oops("read error from", argv[1]); 
	}	

	if(close(input_fd) || close(output_fd) == -1) oops("Error closing files", ""); 


	return 0; 
}


void oops(char* s1, char* s2){
	fprintf(stderr,"Error: %s",  s1); 
	perror(s2);
	exit(-1); 
}
