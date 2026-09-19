#include<stdio.h> 
// #include<stdlib.h> 
#define PAGELEN 5

// getchar -> getc(tty)

int main(int argc, char* argv[]){
	FILE *fp = (argc > 1) ? fopen(argv[1], "r") : stdin; 
	if(fp == NULL){
		perror("fopen error"); 
		return 1; 
	}

	FILE *tty = fopen("/dev/tty", "r"); 
	if(tty == NULL){
		fprintf(stderr, "fopen error in tty"); 
		return 1; 
	}

	char line[256]; 
	int count = 0; 

	while((fgets(line, sizeof(line), fp))){
		//fp -> redirection fd.... 
		fputs(line, stdout); 
		count++; 

		if(count == PAGELEN){
			printf("\033[7m--More(Press enter to continue)--\033[m"); 
			count = 0; 
			getc(tty); //read char from tty(which is actual terminal! )
			// getchar(): get 1 byte from fd 0. (Anything in fd = 0 would be retrived. )
			// IF seq 1 20 | ./mini_pager -> this doesn't mean fd = 0 is empty. fd = 0 is fully covered with 1~ 20. 
		}
	}

	if(fp != stdin)
		fclose(fp); 

	return 0; 
}
