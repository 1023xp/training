/*
 * #define pthread_cleanup_push(routine,arg)                                     \
 *   { struct _pthread_cleanup_buffer _buffer;                                   \
 *       _pthread_cleanup_push (&_buffer, (routine), (arg));
 * #define pthread_cleanup_pop(execute)                                          \
 *   _pthread_cleanup_pop (&_buffer, (execute)); }
 线程为了访问临界资源而为其加上锁，但在访问过程中被外界取消，如果线程处于响应取消状态，且采用异步方式响应，或者在打开独占锁以前的运行路径上存在取消点，则该临界资源将永远处于锁定状态得不到释放。外界取消操作是不可预见的，因此的确需要一个机制来简化用于资源释放的编程。
 pthread_cleanup_push()/pthread_cleanup_pop()函数对用于自动释放资源 --从pthread_cleanup_push()的调用点到pthread_cleanup_pop()之间的程序段中的终止动作（包括调用 pthread_exit()和取消点终止）都将执行pthread_cleanup_push()所指定的清理函数。
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
pthread_mutex_t mutex;
pthread_cond_t  cond;
void * child1(void *arg)
{
//	pthread_cleanup_push(pthread_mutex_unlock,&mutex);  /* comment 1 */
	while(1){
		printf("thread 1 get running \n");
		printf("thread 1 pthread_mutex_lock returns %d\n",
				pthread_mutex_lock(&mutex));
		pthread_cond_wait(&cond,&mutex);
		printf("thread 1 condition applied\n");
		pthread_mutex_unlock(&mutex);
		sleep(5);
	}
//	pthread_cleanup_pop(0);     /* comment 2 */
}
void *child2(void *arg)
{
	while(1){
		sleep(3);               /* comment 3 */
		printf("thread 2 get running.\n");
		printf("thread 2 pthread_mutex_lock returns %d\n",
			pthread_mutex_lock(&mutex));
		pthread_cond_wait(&cond,&mutex);
		printf("thread 2 condition applied\n");
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}
int main(void)
{
	int tid1,tid2;
	printf("hello, condition variable test\n");
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	pthread_create(&tid1,NULL,child1,NULL);
	pthread_create(&tid2,NULL,child2,NULL);
	do{
		sleep(2);                   /* comment 4 */
		pthread_cancel(tid1);       /* comment 5 */
		sleep(2);                   /* comment 6 */
		pthread_cond_signal(&cond);
	}while(1);  
	sleep(100);
	pthread_exit(0);
}


