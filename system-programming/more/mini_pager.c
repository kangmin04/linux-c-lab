#include<stdio.h>
#define PAGELEN 5

/*GOAL : open file by arguemnts and print 5 lines when enter pressed. */

//execution cli : ./mini_pager READFILE.txt 
int main(int argc, char *argv[]){
	FILE* fp = (argc > 1) ? fopen(argv[1], "r") : stdin ; 
	if(fp == NULL){
		perror("fopen"); 
		return 1; 
	}

	// FILE fp read y fgets

	char line[256]; 
	int count = 0; 

	while((fgets(line, sizeof(line), fp)) != NULL ){
		fputs(line, stdout); 
		printf("test %d", count); 
		count++; 

		if(count == PAGELEN){
			count = 0; 
			printf("\033[7m--More(press enter to continue)--\033]m"); 
			fflush(stdout); 
			// read from stdin. IF input is redirectioned, THIS DOESNT WORK. 
			continue; 
		}
	}

	if(fp != stdin){
		fclose(fp); 
	}
	return 0; 
}
