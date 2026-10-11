#include <stdio.h> 
#include <utmp.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> // read, write
#include <fcntl.h> // open file
#include <time.h> 


void who(int is_q); 
void print_utmp_info(struct utmp* info_utmp);
void print_user(struct utmp* info_utmp, int* userNumber);  

void error(char* err_msg, int exit_code){
    fprintf(stderr, "%s\n", err_msg); 
    exit(exit_code); 
}
int main(int ac, char* av[]){
    if(ac == 1) {
        //default who
        who(0); 
        return 0; 
    }   
    if(ac == 2){
        if(strcmp(av[1], "-q")) error("wrong argument", 1); 
        who(1);
    }
    

    return 0; 
}

void who(int is_q){
    struct utmp info_utmp; 
    int reclen = sizeof(struct utmp); 
    int utmp_fd; 
    int userNumber = 0; 
    
    if((utmp_fd = open("/var/run/utmp", O_RDONLY)) == -1)    error("open file error", 1); 
    if(is_q){
        while(read(utmp_fd, &info_utmp, reclen) == reclen){
          print_user(&info_utmp, &userNumber); 
        }
        printf("\n");
        printf("# users = %d\n", userNumber);    
    }else{
        while(read(utmp_fd, &info_utmp, reclen) == reclen){
            print_utmp_info(&info_utmp);
        }
    }
}

void print_utmp_info(struct utmp* info_utmp){ // 궁금증. 원래는 처음에 who 함수 내에서 출력문을 돌렸기에, 이미 info_utmp가 변수로 되어있고, read 지나면 값이 복사되어있으니까 굳이 구조체 변수 포인터없이 . 으로 접근했거든. 근데 기능 분리하면서 함수로 뺴니까 구조체 포인터를 줘서 해야겠더라고? 근데 굳이 사실 그럴필욘 없지않아? 그냥 값 전체르 ㄹ복사해도 뭐.. 메모리는 크긴하지만 이런 학습용에선 . 으로 여전히 가능하지않아?? 물로 ㄴ여기선 포인터로 수정함. 
    if(info_utmp->ut_type != USER_PROCESS) return; 

    printf("%-8.8s", info_utmp->ut_user); 
    printf(" ");
    printf("%-8.8s" , info_utmp->ut_line); 
    printf(" ");
    time_t sec = (time_t)info_utmp->ut_tv.tv_sec; 
    char* convertedTime = ctime(&sec); 
    printf("%12.12s", convertedTime+4); 

    if(info_utmp->ut_host[0] != '\0'){
        printf(" (%s)", info_utmp->ut_host);
    }
    printf("\n");
}

void print_user(struct utmp* info_utmp, int* userNumber){
    if(info_utmp->ut_type != USER_PROCESS) return; 
    printf("%-8.8s", info_utmp->ut_user); 
    (*userNumber)++; 
}