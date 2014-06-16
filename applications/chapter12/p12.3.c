#include <stdio.h>
#include <pthread.h>

void* thread_one_info(void)
{
	int i;

	for(i=0;i<5;i++){
		printf("in thread1, sleep...\n");
		sleep(1);
	}	
	
	printf("thread %ld:in thread_one_info function\n",(long)pthread_self());
		
}

void* thread_two_info(void)
{
	int i;

	for(i=0;i<5;i++){
		printf("in thread2,sleep...\n");
		sleep(1);
	}

        printf("thread %ld:in thread_two_info function\n",(long)pthread_self());
}

int main(int argc,char* argv[])
{
	pthread_t thread_id1,thread_id2;
	int ret;
	struct sched_param param1,param2;
	int num1,num2;	

	if(argc!=3){
		printf("Usage: %s thread_one_priority thread_two_priority\n",argv[0]);
		return 1;
	}

	num1=atoi(argv[1]);
	num2=atoi(argv[2]);

	pthread_attr_t attr_obj1;
	pthread_attr_t attr_obj2;

	pthread_attr_init(&attr_obj1);
	pthread_attr_init(&attr_obj2);
	
	ret=pthread_attr_getschedparam(&attr_obj1,&param1);
	if(ret!=0){
		perror("obj1: cannot get schedule parameters");
		return 1;
	}	

	ret=pthread_attr_getschedparam(&attr_obj2,&param2);
	if(ret!=0){
		perror("obj2: cannot get schedule parameters");
		return 1;
	}

	printf("obj1: thread priority : %d\n",param1.sched_priority);
	printf("obj2: thread priority : %d\n",param2.sched_priority);

	param1.sched_priority=num1;
	param2.sched_priority=num2;

	printf("====now thread priority====\n");
        printf("obj1: thread priority : %d\n",param1.sched_priority);
        printf("obj2: thread priority : %d\n",param2.sched_priority);

        ret=pthread_attr_setschedparam(&attr_obj1,&param1);
        if(ret!=0){
                perror("obj1: cannot set schedule parameters");
                return 1;
        }

        ret=pthread_attr_setschedparam(&attr_obj2,&param2);
        if(ret!=0){
                perror("obj2: cannot set schedule parameters");
                return 1;
        }

	ret=pthread_attr_setscope(&attr_obj1,PTHREAD_SCOPE_SYSTEM);
        if(ret!=0){
                perror("obj1: cannot set schedule scope");
                return 1;
        }

  	ret=pthread_attr_setscope(&attr_obj2,PTHREAD_SCOPE_SYSTEM);
        if(ret!=0){
                perror("obj2: cannot set schedule scope");
                return 1;
        }

  	ret=pthread_attr_setschedpolicy(&attr_obj1,SCHED_RR);
        if(ret!=0){
                perror("obj1: cannot set schedule policy");
                return 1;
        }
  	
	ret=pthread_attr_setschedpolicy(&attr_obj2,SCHED_RR);
        if(ret!=0){
                perror("obj2: cannot set schedule policy");
                return 1;
        }

	ret=pthread_create(&thread_id1,&attr_obj1,(void*)thread_one_info,NULL);
	if(ret==-1){
		perror("cannot create new thread1");
		return 1;
	}

        ret=pthread_create(&thread_id2,&attr_obj2,(void*)thread_two_info,NULL);
        if(ret==-1){
                perror("cannot create new thread2");
                return 1;
        }

	pthread_join(thread_id1,NULL);
	pthread_join(thread_id2,NULL);

	return 0;
}

