// 2023014913 김강민
#include <stdio.h>
#include <time.h>
#include <utmp.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/types.h> 
#include <stdlib.h> 

void error(char* , char* );
int format_record(struct utmp* , char*, size_t) ; 

int main(int argc, char* argv[]){
    ssize_t read_chars; 
    struct utmp current_record; 
    size_t utmp_struct_size = sizeof(struct utmp); 
    char buf[UT_NAMESIZE + UT_LINESIZE + 24 + UT_HOSTSIZE + 1 + 1 + 1]; 
    /*
        뱔도 정보를 파일로 저장하니까... 
    */
    // argc 검사 
    if(argc != 2) error("Usage: %s <output_filename>\n" ,argv[0]);
    // var/run/utmp 파일 open
    int utmp_fd = open(UTMP_FILE, O_RDONLY); 
    if(utmp_fd == -1) {
        perror(UTMP_FILE); 
        exit(1); 
    }
    // argv[1] writefile creat
    int output_fd = creat(argv[1], 0644); 
     if(output_fd == -1) {
        perror(argv[1]); 
        exit(1); 
    }
     
    while((read_chars = read(utmp_fd, &current_record, utmp_struct_size)) == (ssize_t)utmp_struct_size){
        int n = format_record(&current_record, buf, sizeof(buf)); 
        if(n <= 0) continue; 

        if(write(STDOUT_FILENO, buf, n) != (ssize_t)n){
            perror("stdout");
            exit(1); 
        }

        if(write(output_fd, buf, n) != (ssize_t)n){
            perror("outputFile");
            exit(1); 
        }
    }

    if(read_chars == -1){
          perror("read");
          exit(1); 
    } else if (read_chars > 0) fprintf(stderr, "warning: truncated record (%zd bytes) ignored\n", read_chars);

    //close
     if(close(utmp_fd) == -1){
        perror("utmp");
        exit(1);
    }
    if(close(output_fd) == -1){
        perror(argv[1]);
        exit(1);
    }
    return 0; 
}


int format_record(struct utmp* ut_buf_pointer, char* out, size_t out_size){
    if(ut_buf_pointer->ut_type != USER_PROCESS) return 0; 
    time_t sec = ut_buf_pointer->ut_tv.tv_sec; 

    int n = snprintf(out, out_size, "%-8.8s %-8.8s %12.12s (%.*s)\n",  // %s (%s)\n으로 끝에 두개를 교수님이 하심.  
        ut_buf_pointer->ut_user, 
        ut_buf_pointer->ut_line, 
        ctime(&sec)+4,
        UT_HOSTSIZE, ut_buf_pointer->ut_host
    ); 
    // 교수님은 그냥 wirte(fd, write_buf, len) != len)으로 멈춤. 
    
    if(n < 0 || (size_t)n >= out_size){
        fprintf(stderr, "format error\n"); 
        return -1; 
        // exit(1); 오류탐지와 판단은 다른 층에서 하는게 바람직함. exit을 호출할경우, 테스트, 열어둔 fd정리, 재사용불가, 에러 메시지 실종 등 문제가 있다. 
        // exit은 주로 main이 직접 호출할 때 호출함. 
    }

    return n; 
}

void error(char* errorLog, char* argv){
    fprintf(stderr, errorLog, argv);
    exit(1);
}


/*
    save_info()

    chat write_buf[256] = {0}
    char time_str[20] = {0}
    strncpy로 복사
    snprintf -> 문자열로 출력. 형식화된 문자열 만들 대. 

*/

