// 1-1. mycp -i 구현 (hw02 응용)
// - ./mycp -i src dest를 실행했을 때 dest가 이미 있으면 overwrite dest? (y/n) 를 출력하고 키보드로 답을 받는다.
// - 표준입력이 리다이렉트돼 있어도 답은 키보드에서 받아야 한다. (./mycp -i a b < something)
// - y면 복사하고, 그 밖의 답이면 복사하지 않고 끝낸다.
// - 원본과 대상이 같은 파일이면 거부한다. 이때 strcmp가 아니라 stat으로 판별해야 한다. (./mycp a ./a도 걸러져야 함)

/*
    ssize_t read(int fd, void *buf, size_t nbytes)
    int open(char* path, int flags, mode_t mode)
    int creat(char* path, mode_t) -> 성공 시 fd, 에러면 -1
    read, write : ssize_t반환. 에러 -1, 파일끝이면 0 
    int stat(char* path, struct stat* buf) 성공시 0 아니면 -1

    cp에서 가장 핵심적인 포인트: 
    while((n_chars = read(input_fd, buf, buffersize)) > 0){
        write(output_fd, buf, n_chars) 실제로 읽은 크기만큼을 작성해야함. buffersize, sizeof(buf) 라면 실제 읽은거 보다 더 많이 작성하게되서 다른 메모리가 가리키는 쓰레기값들이 들어옴. 
    }
*/

#include <stdio.h> 
#include <unistd.h> // 시스템콜(read, write, close 등등)
#include <fcntl.h> // open, creat, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC 등.. 
#include <sys/stat.h> 
#include <stdlib.h> // exit
#include <string.h> 

#define BUFFERSIZE 1024

void cpCompany(char* , char* ); 

int main(int argc, char* argv[]){
    if(argc < 3  || argc > 4){
        perror("Invalid arguments.\n");
        exit(1);  
    }

    if(argc == 3){
        //No option. 

        cpCompany(argv[1], argv[2]); 
        return 0; 
    }

    //argc 4 case
   
    //디렉토리에 존재하는지를 체크헤야함 !!! 이미 존재할 경우 overwrite dest? (y/n) 를 출력하고 키보드로 답을 받는다.
    // argv[3]에 대한 디렉토리 조사가 필요하다.....
    struct stat stat_buf; 
    FILE* fp = fopen("/dev/tty", "r"); 
    if(fp ==NULL){
        perror("failed to read /dev/tty");
        exit(1); 
    }

    int c = 0; 
    struct stat srcBuf; 
    if((stat(argv[3], &stat_buf)) == 0 ){ // stat으로 내가 지정한 위치에 해당 파일이 존재하는지 여부 체크함. 
         if(stat(argv[2], &srcBuf) == -1) perror("error argv stat"); 
                if(srcBuf.st_dev == stat_buf.st_dev && srcBuf.st_ino == stat_buf.st_ino){
                    perror("It is the same file"); 
                    exit(1); 
                }
         if(strcmp(argv[1], "-i") != 0){ // strcmp는 같으면 0 
            fprintf(stderr, "it is not -i. option"); 
            exit(1); 
        }
        printf("overwrite dest? (y/n)"); 
        if((c = getc(fp)) != EOF){
            if(c == 'y'){
                //overwrite
               
                cpCompany(argv[2], argv[3]); 
            } else if(c == 'n'){
                perror("file already exists"); 
                exit(1); 
            }

        }
        
    }else{
        //normal cp 
        cpCompany(argv[2], argv[3]);
    }
    return 0; 
}

void cpCompany(char* src, char* dest){
    //input_fd, output_fd(creat), read from input -> write to output. -> close 
    int input_fd; 
    int output_fd;
    char buf[BUFFERSIZE]; 
    ssize_t n_chars; 

    if((input_fd = open(src, O_RDONLY)) == -1 ){
        fprintf(stderr,"Failed to open %s\n", src); 
        exit(1); 
    }

    if((output_fd = creat(dest, 0644)) == -1){
        fprintf(stderr,"Failed to create %s\n", dest); 
        exit(1); 
    }

    while((n_chars=read(input_fd, buf, BUFFERSIZE )) > 0) {
        if((write(output_fd, buf, n_chars)) != n_chars){
            perror("write error"); 
            exit(1); 
        }
    }

    close(input_fd); 
    close(output_fd); 
}