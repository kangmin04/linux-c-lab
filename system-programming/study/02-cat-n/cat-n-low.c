#include <stdio.h>  // fprintf, snprintf, perror
#include <stdlib.h> // exit
#include <string.h> // strcmp
#include <fcntl.h>  // open, O_RDONLY
#include <unistd.h> // read, write, close, STDOUT_FILENO

#define BUFFERSIZE 256
int main(int argc, char* argv[]){
    if(argc != 3 || strcmp(argv[1], "-n") != 0){
        fprintf(stderr, "usage: %s -n file\n", argv[0]);
        exit(1);
    }
    int input_fd;
    if((input_fd = open(argv[2], O_RDONLY)) == -1){
        perror(argv[2]);
        exit(1);
    }

    char buf[BUFFERSIZE]; // 파일에서 읽은 내용
    char num[16];         // 줄 번호를 "     1\t" 같은 문자열로 만들어 담는 칸
    int len;              // num에 실제로 들어간 글자 수
    int lineNumber = 1;
    int atLineStart = 1;
    ssize_t n_chars;

    while ((n_chars = read(input_fd, buf, BUFFERSIZE)) > 0) {
        for (int i = 0; i < n_chars; i++) {
            // 매 바이트를 \n인지 검사만 해주고, 계속 한 바이트씩 출력해줌. 
            // 개행이면 그냥 다음거로 넘어감. -> atLineStart로 다음줄. 
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
    close(input_fd);
    return 0;
}