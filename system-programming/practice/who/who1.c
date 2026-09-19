#include <stdio.h>
#include <string.h>
#include <time.h>
#include <utmp.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/types.h> 
#include <stdlib.h> 



void show_info(struct utmp * ); 

int main(){
	struct utmp current_record; 
	int utmpfd; 
	size_t rec_len = sizeof(struct utmp); 
	
	if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1 ){
		perror(UTMP_FILE); 
		exit(EXIT_FAILURE); 
	}
	ssize_t read_chars; 
	while((read_chars= read(utmpfd, &current_record, rec_len)) == (ssize_t)rec_len){
			show_info(&current_record);
		}
	
	if(read_chars == -1){
			perror("readFileError"); 
			exit(EXIT_FAILURE); 
		}

	close(utmpfd); 
	return 0; 
}

void show_info(struct utmp* ut_buf_pointer){
	if(ut_buf_pointer->ut_type != USER_PROCESS) return; 

	printf("%-8.8s " , ut_buf_pointer->ut_user); 
	printf("%-8.8s " , ut_buf_pointer->ut_line); 
	
	time_t sec = ut_buf_pointer->ut_tv.tv_sec; 
	char *ctime_str = ctime(&sec); 
	ctime_str[strlen(ctime_str)-1] = '\0'; 
	printf("%s ", ctime_str); 
	printf("(%s)", ut_buf_pointer->ut_host); 
	printf("\n"); 
}


