#include <stdio.h> 
#include <stdlib.h> // exit
#include <string.h>  // strlen
#include <fcntl.h> 

#define BUFFERSIZE 256
int main(int argc, char* argv[] ){
    if(argc != 3){
        fprintf(stderr, "error"); 
        exit(1); 
    }
    int input_fd; 
    if((input_fd=open(argv[2],O_RDONLY ))== -1){
        perror("Failed to open file"); 
        exit(1); 
    }
    // char *fgets(char *str, int num, FILE *stream);
    char buf[BUFFERSIZE]; // buf 크기를 초과할 때를 생각못했음. 
    int lineNumber = 1; 
    int atLineStart = 1; 
    ssize_t n_chars; 

    while ((n_chars = read(input_fd, buf, BUFFERSIZE)) > 0) {
        for (int i = 0; i < n_chars; i++) {
            if (atLineStart) {
                // int snprintf(char *str, size_t size, const char *format, ...);
                len = snprintf(num, sizeof(num), "%6d\t", lineNumber++);
                write(STDOUT_FILENO, num, len);
                atLineStart = 0; 
            }
            write(STDOUT_FILENO, &buf[i], 1);
            if (buf[i] == '\n')
                atLineStart = 1;
        }
    }
    if (n_chars == -1) { perror(argv[2]); exit(1); }
    return 0;

}



