#include <stdio.h>
#include <pthread.h>

struct thread_param
{
	char info;
	int num;
};

void* thread_fun(void* param)
{
	struct thread_param* p;
	
	p=(struct thread_param*)param;
	int i;

	for(i=0;i<p->num;i++){
		printf("%i: %c\n",i,p->info);
	}
		
	return NULL;
}

int main(void)
{
	pthread_t tid1,tid2;
	struct thread_param info1;
	struct thread_param info2;
	int ret;

	info1.info='T';
	info1.num=2;

	ret=pthread_create(&tid1,NULL,thread_fun,&info1);
	if(ret==-1){
		perror("cannot create new thread");
		return 1;
	}

	info2.info='S';
	info2.num=3;
	
	ret=pthread_create(&tid2,NULL,thread_fun,&info2);
	if(ret==-1){
		perror("call pthread_join function fail");
		return 1;
	}

	if(pthread_join(tid1,NULL)!=0){
		perror("call pthread_join function fail");
		return 1;
	}
	
        if(pthread_join(tid2,NULL)!=0){
                perror("call pthread_join function fail");
                return 1;
        }
	return 0;
}	
