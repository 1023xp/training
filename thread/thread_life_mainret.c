#include <stdio.h>
#include <pthread.h>

/* 
 * uncomment this to enable main join and ret
 */

//#define MAIN_JOIN
//#define MAIN_RET

void* thread_fun(void* param)
{
	while(1);
	return NULL;
}

int main(void)
{
	int ret;
	pthread_t tid1,tid2;

	ret=pthread_create(&tid1,NULL,thread_fun,NULL);
	if(ret==-1){
		perror("cannot create new thread");
		return 1;
	}

	ret=pthread_create(&tid2,NULL,thread_fun,NULL);
	if(ret==-1){
		perror("cannot create new thread");
		return 1;
	}
#if defined(MAIN_JOIN)
	if(pthread_join(tid1,NULL)!=0){
		perror("call pthread_join function fail");
		return 1;
	}

	if(pthread_join(tid2,NULL)!=0){
		perror("call pthread_join function fail");
		return 1;
	}
#elif defined(MAIN_RET)
	return 0;
#else	
	pthread_exit(0);
#endif
}
