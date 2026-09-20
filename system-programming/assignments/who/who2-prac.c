// 2023014913 김강민
#include <stdio.h>
#include <time.h>
#include <utmp.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/types.h> 
#include <stdlib.h> 

void error(char* , char* );
// void show_info(struct utmp * , FILE* stream); 
// 난 처음에 outputfd로 fprintf에서 스트림으로 사용해서, show_info에 출력스트림만 다르게 주어, strategy pattern으로 작성하려했는데, fprintf는 fd가 아닌, stream(file* )만 받음. fd는 여전히 write 해줘야함. 

// void show_info(struct utmp *); 
int format_record(struct utmp* , char*, size_t) ; 

int main(int argc, char* argv[]){
    ssize_t read_chars; 
    struct utmp current_record; 
    size_t utmp_struct_size = sizeof(struct utmp); 
    char buf[UT_NAMESIZE + UT_LINESIZE + 24 + UT_HOSTSIZE + 1 + 1 + 1]; 
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
    
    // open한 파일 그대로 argv[2]에 작성 + 터미널 출력  
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

        
        //터미널에 출력 - show_info에 stdout으로 스트림 지정
        // show_info(&current_record, stdout); 

        //파일에 작성 - show_info에 file의 fd를 주자. 
        // if(write(output_fd, &current_record, read_chars) != read_chars){
        //     perror(argv[1]);
        //     exit(1);
        // }
        /*
            1. show_info에서 문자열 조립해서 리턴. -> 이 방식은 기능을 한 함수에 2개 구현하게됨. 
            2. 차라리 뮌자열 조립이란 함수를 두고, 문자열 조립해서 그걸 str 형태로 바로 각각의 출력에 넣는 방식? 차라리 이게 나을듯. 
        */
        // show_info(&current_record, output_fd); 
    }

    if(read_chars == -1){
          perror("read");
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

    int n = snprintf(out, out_size, "%-8.8s %-8.8s %12.12s (%.*s)\n", 
        ut_buf_pointer->ut_user, 
        ut_buf_pointer->ut_line, 
        ctime(&sec)+4,
        UT_HOSTSIZE, ut_buf_pointer->ut_host
    ); 

    if(n < 0 || (size_t)n >= out_size){
        perror("formating problem"); 
        return -1; 
        // exit(1); 오류탐지와 판단은 다른 층에서 하는게 바람직함. exit을 호출할경우, 테스트, 열어둔 fd정리, 재사용불가, 에러 메시지 실종 등 문제가 있다. 
        // exit은 주로 main이 직접 호출할 때 호출함. 
    }

    return n; 
}

// void show_info(struct utmp* ut_buf_pointer, FILE* stream ){
// 	if(ut_buf_pointer->ut_type != USER_PROCESS) return; 

// 	printf( "%-8.8s " , ut_buf_pointer->ut_user); 
// 	printf( "%-8.8s " , ut_buf_pointer->ut_line); 
	
// 	time_t sec = ut_buf_pointer->ut_tv.tv_sec; 
// 	char *ctime_str = ctime(&sec); 
// 	ctime_str[strlen(ctime_str)-1] = '\0'; 
// 	printf( "%s ", ctime_str); 
// 	printf("(%s)", ut_buf_pointer->ut_host); 
// 	printf( "\n"); 
// }


void error(char* errorLog, char* argv){
    fprintf(stderr, errorLog, argv);
    exit(1);
}