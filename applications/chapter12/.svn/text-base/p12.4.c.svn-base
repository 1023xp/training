#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int g_var;

//initialize the mutex
pthread_mutex_t locker=PTHREAD_MUTEX_INITIALIZER;

void *thread_function(void) 
{

      	int i,j;
  
	for (i=0;i<4;i++) {
		pthread_mutex_lock(&locker);
		
		j=g_var;
		j=j+1;
		printf("---in thread...increase one---\n");
		fflush(stdout);
		sleep(1);
		g_var=j;
		
		pthread_mutex_unlock(&locker);
	}
  
	return NULL;
}

int main(void) {
	pthread_t thread_id;
	int i;
  
	if (pthread_create(&thread_id,NULL,thread_function,NULL)) {
		perror("cannot create new thread");
		return 1;
	}
  
	for (i=0;i<4;i++){
		pthread_mutex_lock(&locker);
		
		g_var=g_var+1;
		
		pthread_mutex_unlock(&locker);
    
		printf("...in main thread...increase one...\n");
		fflush(stdout);
    
		sleep(1);
	}
  
	if (pthread_join (thread_id,NULL)){
		perror("cannot join thread.");
		return 1;
	}
  
	printf("g_var: %d\n",g_var);
  
	exit(0);
}

