#include <stdio.h> 
#include <stdlib.h> // exit
#include <string.h>  // strlen

int main(int argc, char* argv[] ){
    if(argc != 3){
        perror("error"); 
        exit(1); 
    }

    FILE* fp;
    //FILE* fopen(const char* filename, const char* mode); 
    if((fp = fopen(argv[2], "r")) == NULL){
        perror("Failed to open file"); 
        exit(1); 
    }
    // char *fgets(char *str, int num, FILE *stream);
    char buf[256]; // buf 크기를 초과할 때를 생각못했음. 
    int lineNumber = 1; 
    int atLineStart = 1; 
    while(fgets(buf, sizeof(buf), fp) != NULL){
        if(atLineStart){
            printf("%6d\t", lineNumber++); 
        }
        fputs(buf, stdout); 
        // 결국 마지막인지 체크하는건 맨 마지막에 읽은 buf값이 \n인지 아닌지로 여부 체크함. 
        atLineStart = (buf[strlen(buf)-1] == '\n');
    }
    fclose(fp); 
    return 0;

}