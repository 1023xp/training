#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include "swap.h"
#include "apple.h"

void* swapapple(void* x)
{
	swapvec((struct number *)x);
}

void* addapple(void* x)
{
	int sum;
	sum = addvec((struct number *)x);
	printf("sum = %d\n",sum);
}


int main()
{
    int x = 12 ,y = 90;
    struct number apple;
	pthread_t ThreadA,ThreadB;
	pthread_rwlock_t aaa;
    apple.x= x;
	apple.y= y;
	pthread_rwlock_init(&apple.rwLock,NULL);
	pthread_create(&ThreadA,NULL,swapapple,&apple);
	pthread_create(&ThreadB,NULL,addapple,&apple);
    pthread_join(ThreadA,NULL);
    pthread_join(ThreadB,NULL);
	printf("x = %d,y = %d\n",apple.x,apple.y);
	return 0;
}

#ifdef __cplusplus
}
#endif
