/*
 * int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *, void *arg)); 
 * (the address where new thread's id will be stored, attr is usually NULL, start_routine : function that thread will excute MUST void* finc(void *arg), )
 *
 * int pthread_join(pthread_t, void **retval); 
 *
 *
 * */


#include<stdio.h>
#include<pthread.h>

void *say_hi(void *arg){  // FUNCTION POINT : function's address. -> SO it can be stored to other variable (void* A = say_hi) OR can be sent to other function's argument. 
	(void)arg; //INDICATE TO COMPILER THAT ARG WILL NEVER USED. 
	printf("HI FROM A THREAD\n"); 
	return NULL;  // IF return doesn't need, then just use NULL
}

int main(void){
	pthread_t t; 
	pthread_create(&t, NULL, say_hi, NULL); 
	pthread_join(t, NULL); // when return value doesn't necessary -> USE NULL
	printf("MAIN THREAD DONE\n"); 
	return 0; 
}
