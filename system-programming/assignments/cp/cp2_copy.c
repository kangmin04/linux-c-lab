// 2023014913 김강민

#include<fcntl.h> // file 생성 플래그 (O_CREAT, .. )
#include<unistd.h> // 시스템콜(read, open, write.. ) 
#include<stdio.h> 
#include<stdlib.h>

#define BUFFERSIZE 1024


void error(char* , char* ); 

int main(int argc, char* argv[]){

	char buf[BUFFERSIZE]; 
	ssize_t  n_chars; 
	long long totalRead = 0; // long long : 8바이트  
	/*
		int : 4바이트 -> 2^32 = 양수 최대값 21억. 만약 2GB이상의 파일이 반환되면 못읽음. 
		메모리크기, 요청바이트 수: size_t
		한번의 IO 결과(바이트 수 or -1) : ssize_t 
		파일크기 : off_t
	 */

	printf("Test: argv[2] : %s", argv[2]); 

	if(argc != 3)	error("Usage: %s  source  destination" ,argv[0]); 

	// open file from sourcetxt
	int input_fd = open(argv[1], O_RDONLY);
	if(input_fd == -1)	error("failed to open file. Check file existence or name.", " ");

	// create file whether it already exists or not. 
	int output_fd = creat(argv[2], O_WRONLY);
	if(output_fd == -1) error("failed to create file", " ");

	// read한걸 write해야함. 
	//read : 읽은 바이트 수 출력 | EOF = 0 | error시 -1
	while((n_chars = read(input_fd, buf, BUFFERSIZE) ) > 0 ){	
		write(output_fd, buf, n_chars); 
		printf("N_chars : %zd", n_chars); 
		totalRead += n_chars; 
		printf("total read : %lld", totalRead); 
	}

	if(n_chars == -1) error("failed to read input file", argv[1]); 


	if(((close(input_fd) == -1 ) ||( close(output_fd) == -1))) error("failed to close file", " " );  
	
	printf("total read bytes: %lld", totalRead); 

	return 0; 
}

void error(char* errorLog, char* argv){
	fprintf(stderr, errorLog, argv); 
	exit(1); 
}
