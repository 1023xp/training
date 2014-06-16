#include <pthread.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <unistd.h>  
#include <string.h>  
pthread_mutex_t mymutex1 = PTHREAD_MUTEX_INITIALIZER;  
//pthread_mutex_t mymutex2 = PTHREAD_MUTEX_INITIALIZER;  
pthread_cond_t mycond = PTHREAD_COND_INITIALIZER;  
void *mythread1(void *param)  
{  
	printf("begin mythread1.\n");  
	pthread_mutex_lock(&mymutex1);  
	printf("wait in mythread1.\n");  
	pthread_cond_wait(&mycond,&mymutex1);  
	pthread_mutex_unlock(&mymutex1);  
	printf("end mythread1.\n");  
	return NULL;  
}
  
void *mythread2(void *param)  
{  
	printf("begin mythread2.\n");  
	pthread_mutex_lock(&mymutex1);  
	printf("wait in mythread2.\n");  
	pthread_cond_wait(&mycond,&mymutex1);  
	pthread_mutex_unlock(&mymutex1);  
	printf("end mythread2.\n");  
	return NULL;  
}
  
int main(void)  
{  
	printf("begin main thread.\n");  
	int i;  
	pthread_t tid1,tid2;  
	pthread_create(&tid1,NULL,mythread1,NULL);  
	pthread_create(&tid2,NULL,mythread2,NULL);  
	sleep(2);  
	printf("try to wake up mythread1 and mythread2 in main thread.\n");  
	if(pthread_cond_broadcast(&mycond)){  
		printf("error\n");  
		return 1;  
	}  
	void *res;  
	pthread_join(tid1, &res);  
	pthread_join(tid2, &res);  
	printf("end main thread.\n");  
	return 0;  
}  
