#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
pthread_mutex_t mutex_1;
pthread_mutex_t mutex_2;

void *child1(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex_1);
		sleep(3);
		pthread_mutex_lock(&mutex_2);
		printf("thread 1 get running \n");
		pthread_mutex_unlock(&mutex_2);
		pthread_mutex_unlock(&mutex_1);
		sleep(5);
	}
}

void *child2(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex_2);
		pthread_mutex_lock(&mutex_1);
		printf("thread 2 get running \n");
		pthread_mutex_unlock(&mutex_1);
		pthread_mutex_unlock(&mutex_2);
		sleep(5);
	}
}

int main(void)
{
	int tid1,tid2;
	pthread_mutex_init(&mutex_1,NULL);
	pthread_mutex_init(&mutex_2,NULL);
	pthread_create(&tid1,NULL,child1,NULL);
	pthread_create(&tid2,NULL,child2,NULL);
	do{
		sleep(2);
	}while(1);
	pthread_exit(0);
}

