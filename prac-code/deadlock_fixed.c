/* 
 * code flow : thread1 -> (thread1's lockA occurs) -> (thread2 was also trying to get lock A but they failed. then they just SLEEP and waiting for unlock(A)  -> thread1's unlock(A) -> thread2 starts. 
 *
 * pthread_mutex_t : struct that incldues flag, owner, pending threads info etc..
 * So when we handle mutex, we give its address and by using copy bv value, we can access to inner member.
 * PTHREAD_MUTEX_INITIALIZER = macro. it looks like {0,0,0,0,.....}. this belps to initialize flag and queue to empty.  
 *  UNLOCK ORDER : LIFO. when u get lock A -> B then unlock B -> A ! 
 *  this will prevent unnecessary context switching. 
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
	printf("thread2: lock_a LOCK TRY\n");  // lock_a !  
	pthread_mutex_lock(&lock_a); 
	printf("thread2 : lock_a GET\n");  

	sleep(1); 

	
	printf("thread2: lock_b LOCK TRY\n"); 
	pthread_mutex_lock(&lock_b);
	printf("thread2 : lock_b GET\n");

	pthread_mutex_unlock(&lock_b); 	
	pthread_mutex_unlock(&lock_a);
	 	
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
