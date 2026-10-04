/*
 * thread1 hold lock_a and wait for lock_b's release and thread2 hold lock_b and wait for lock_a's release. 
 * so they are terminated !!! 
 * never print "EVERY THREAD DONE" 
 * instead print exit code 124 to indicate terminated by timeout event. 
 * */





#include<stdio.h>
#include<unistd.h> 
#include<pthread.h>

pthread_mutex_t lock_a = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t lock_b = PTHREAD_MUTEX_INITIALIZER; 

void *thread1_func(void *arg){
	(void)arg; 
	printf("thread1: lock_a LOCK TRY\n"); 
	pthread_mutex_lock(&lock_a); 
	printf("thread1 : lock_a GET\n");  

	sleep(1); 

	
	printf("thread1: lock_b LOCK TRY\n"); 
	pthread_mutex_lock(&lock_b);
	printf("thread1 : lock_b GET\n");

	pthread_mutex_unlock(&lock_b); 	
	pthread_mutex_unlock(&lock_a);
 	return NULL; 	

}

void *thread2_func(void *arg){
	(void)arg; 
	printf("thread2: lock_b LOCK TRY\n"); 
	pthread_mutex_lock(&lock_b); 
	printf("thread2 : lock_b GET\n");  

	sleep(1); 

	
	printf("thread2: lock_a LOCK TRY\n"); 
	pthread_mutex_lock(&lock_a);
	printf("thread2 : lock_a GET\n");

	pthread_mutex_unlock(&lock_a); 	
	pthread_mutex_unlock(&lock_b);
	 	
	return NULL; 
}
int main(void){
	pthread_t t1, t2;

	pthread_create(&t1,NULL, thread1_func, NULL); 	
	pthread_create(&t2,NULL, thread2_func, NULL); 
	
	pthread_join(t1, NULL); // WHEN deadlock happens, this join never executed.  
	pthread_join(t2, NULL); 

	printf("EVERY THREAD DONE\n"); 
	return 0;
	

}
