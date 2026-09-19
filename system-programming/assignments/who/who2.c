// 2023014913 김강민
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <utmp.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/types.h> 
#include <stdlib.h> 


int main(int argc, char* argv[]){
    // argc 검사 
    // var/run/utmp 파일 open
    // argv[2] writefile creat
    // open한 파일 그대로 argv[2]에 작성 + 터미널 출력  
    return 0; 
}