#include<stdio.h>

int main(void){
	FILE *fp = fopen("greeting2.txt", "w");
	if(fp ==NULL){
		return 1; 
	}

	fprintf(fp, "Hello, FILE*! \n"); // buffered..
	fclose(fp); // buffer flush and actual write event happens

	return 0;
}
