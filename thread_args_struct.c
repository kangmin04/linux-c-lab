
/*
 * int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *, void *arg)); 
 * (the address where new thread's id will be stored, attr is usually NULL, start_routine : function that thread will excute MUST void* finc(void *arg), )
 *
 * int pthread_join(pthread_t, void **retval); 
 *
 *
 * */


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct {
	int a;
	int b; 
} add_args_t; 


void *add_thread(void *arg){  // FUNCTION POINT : function's address. -> SO it can be stored to other variable (void* A = say_hi) OR can be sent to other function's argument. 
	add_args_t *p = (add_args_t *)arg; // type convert, since start_routine only allows (void *) format.  
	
	int *sum = malloc(sizeof(int)); // heap is shared in thread !  IF we just use int sum -> then it's in stack and versatil when thread done. 
	*sum = p->a + p->b; 
	return sum; // the address where 7 is stored. 
}

int main(void){
	pthread_t t; 
	add_args_t args = {.a = 3, .b = 4};

	pthread_create(&t, NULL, add_thread, &args);
	
	void* result; //pointer variable 
	pthread_join(t, &result) ;  // pointer variable's address is passed by value(copy). and when it excuted, os will insert to &result the finished thread's return value address.  --> result has the address where 7 is stored.  
	printf("3 + 4 = %d\n", *(int *)result); // result type: void *. SO let compiler know it is (int *) and, we use * to get actual value.
        free(result); // release memory in main. NOT IN MALLOC EXECUTED. 	
	return 0; 
}
