#include<stdio.h>
#include<pthread.h> 

#define NUM_THREADS 4
#define ITERATIONS 1000000

long counter = 0;  // global variable that every thread will access
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

void *increment(void* arg){
	(void)arg; 
	for (int i=0; i < ITERATIONS ; i++){
		pthread_mutex_lock(&lock); 
		counter++; 
		pthread_mutex_unlock(&lock); 
	}

	return NULL; 
}

int main(void){
	pthread_t threads[NUM_THREADS]; 
	
	for(int i=0; i < NUM_THREADS; i++){
		pthread_create(&threads[i], NULL, increment, NULL); 			
	}

	for(int i =0 ; i < NUM_THREADS ; i++){
		pthread_join(threads[i], NULL); 
	}

	long expected = (long)NUM_THREADS * ITERATIONS; 
	printf("expected: %ld\n", expected); 
	printf("counter: %ld\n",  counter); 
}
