#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>

void* thread_fun(void* p)
{
	int policy;
	int max_priority,min_priority;
	struct sched_param param;
	pthread_attr_t attr;

	pthread_attr_init(&attr); /*初始化线程属性变量*/
	pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED); /*设置线程继承性*/
	pthread_attr_getinheritsched(&attr,&policy); /*获得线程的继承性*/

	if(policy==PTHREAD_EXPLICIT_SCHED)
		printf("Inheritsched:PTHREAD_EXPLICIT_SCHED\n");

	if(policy==PTHREAD_INHERIT_SCHED)
		printf("Inheritsched:PTHREAD_INHERIT_SCHED\n");

	pthread_attr_setschedpolicy(&attr,SCHED_RR);/*设置线程调度策略*/
	pthread_attr_getschedpolicy(&attr,&policy);/*取得线程的调度策略*/

	if(policy==SCHED_FIFO)
		printf("Schedpolicy:SCHED_FIFO\n");

	if(policy==SCHED_RR)
		printf("Schedpolicy:SCHED_RR\n");

	if(policy==SCHED_OTHER)
		printf("Schedpolicy:SCHED_OTHER\n");

	max_priority = sched_get_priority_max(policy);/*获得系统支持的线程优先权的最大值*/
	min_priority = sched_get_priority_min(policy);/* 获得系统支持的线程优先权的最小值*/

	printf("Max priority:%u\n",max_priority);
	printf("Min priority:%u\n",min_priority);

	param.sched_priority=max_priority;
	pthread_attr_setschedparam(&attr,&param);/*设置线程的调度参数*/
	printf("sched_priority:%u\n",param.sched_priority);/*获得线程的调度参数*/
	pthread_attr_destroy(&attr);
	printf("thread pid:%d, tid:%lu\n",getpid(), pthread_self());
	return NULL;
}

int main(void)
{
	pthread_t tid;
	int ret;

	ret=pthread_create(&tid,NULL,thread_fun,NULL);
	if(ret==-1){
		perror("cannot create new thread");
		return 1;
	}

	if(pthread_join(tid,NULL)!=0){
		perror("call pthread_join function fail");
		return 1;
	}

	return 0;
}	
