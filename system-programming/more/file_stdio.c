#include<stdio.h> 

int main(void){

	int ch;  // NOT CHAR. when getchar reached end, it returns EOF. EOF often regarded as -1 and it return with 32 bits. . char is 8 bit and (-128~ 127 OR 0 ~ 255) so when we convert 32 bits to 8 bits, we got loss for 3bytes. SO.. -1 is 0xFFFFFFFF -> 0xFF. 
		 // char is regarded diffrently depends on os, compiler. THEY HAVE LOSS! so instead, we use 32 bits int. 
	printf("ENTER RANDOM CHAR\n"); 
	ch = getchar(); 
	

	printf("YOUR CHAR IS : "); 
	putchar(ch); 
	putchar('\n'); 

	printf("----------\n"); 

	char line[256]; 
	printf("WHAT IS UR NAME? : "); 
	while(fgets(line, sizeof(line), stdin) != NULL){ // MUST USE fgets rather than gets. gets doesn't get buffer size and if input is larger than buffer, nearby data might corrupted. 
		printf("UR NAME : %s\n", line); 
	}


	fputs("THIS LINE IS BY FPUTS", stdout); 
	return 0; 
}
