#include <stdio.h>
#include <pthread.h>

void* thread_info(void)
{
	fprintf(stderr,"in thread_info function\n");	
}


int main(void)
{
	pthread_t thread_id;
	int ret;

	ret=pthread_create(&thread_id,NULL,(void*)thread_info,NULL);
	if(ret==-1){
		perror("cannot create new thread");
		return 1;
	}

	sleep(5);
	return 0;
}

