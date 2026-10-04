#include<stdio.h>


void one_pointer(int* p){
	*p = 100; 
}

void second_pointer(int** p){
	int result = 200; 
	*p = &result; 

}



int main(void){
	int a ; 	
	one_pointer(&a); 
	printf("one pointer: %d\n", a); 

	int *b; 
	second_pointer(&b);
	printf("second pointer: %d\n", *b ); 
	return 0; 
}
