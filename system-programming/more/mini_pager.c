#include<stdio.h>
#define PAGELEN 5

int main(int argc, char *argv[]){
	FILE *fp = (argc > 1) ? fopen(argv[1], "r"): stdin; 
	if(fp == NULL){
		perror("fopen"); 
		return 1; 
	}

	char line[256]; 
	int count = 0; 

	while(fgets(line, sizeof(line), fp) != NULL){
		fputs(line, stdout); 
		count++; 

		if(count == PAGELEN){
			printf("\033[7m--More(Press enter to continue)--\033[m"); 
			printf("\n ---------------- \nBEFORE FLUSH\n");
			fflush(stdout); 
			printf("\n AFter flash\n"); 
			getchar(); 
			count = 0; 

		}
	}

	if(fp != stdin){
		fclose(fp); 
	}
	return 0; 
}
