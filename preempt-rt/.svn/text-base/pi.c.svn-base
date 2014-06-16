#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

#define USE_PRI_INHERIT /* high-priority task only wait LOW_SPIN time */
#if 0
#undef USE_PRI_INHERIT /* high-priority task wait LOW+MID_SPIN time */
#endif

/*
   A simple demonstration of a 3-thread priority inversion that can
   be improved with priority inheritance.

   Written by Kevin Dankwardt, k@kcomputing.com

 */

sem_t low_go, mid_go, high_go, high_ready, mid_ready;
pthread_mutex_t shared_mutex;
const int minutes = 60;
const int seconds = 1;
pthread_mutexattr_t mutex_attr;

#define LOW_SPIN 2
#define MID_SPIN 5

int gettime ()
{
	return time(NULL);
}


void spin_for (int n)
{
	int now = gettime();
	int counter=0;
	while ( ( gettime() - now) < n)
	{ counter++;
		//if ((counter % 1000000) == 0) printf("gettime()-now = %d n=%d\n",gettime()-now,n);
	}
	//printf("done spinning for %d seconds\n",n);
}

void *low (void *n)
{
	int now = gettime();
	struct sched_param the_priority;

	the_priority.sched_priority  = 1;
	pthread_setschedparam(pthread_self(), SCHED_FIFO, &the_priority);
	sem_wait(&low_go);
	pthread_mutex_lock(&shared_mutex);
	sem_wait(&mid_ready);
	sem_wait(&high_ready);
	sem_post(&high_go);
	sem_post(&mid_go);
	spin_for(LOW_SPIN*seconds);
	pthread_mutex_unlock(&shared_mutex);
	printf ("low took %d seconds wanted about %d (critical section + mid time)\n",gettime() - now,LOW_SPIN+MID_SPIN);
	return NULL;
}

void *mid (void *n)
{
	struct sched_param the_priority;
	int now;

	the_priority.sched_priority  = 25;
	pthread_setschedparam(pthread_self(), SCHED_FIFO, &the_priority);
	sem_post(&mid_ready);
	sem_wait(&mid_go);
	now = gettime();
	spin_for(MID_SPIN*seconds);
	printf ("mid took %d seconds wanted about %d\n",gettime() - now,MID_SPIN);
	return NULL;
}

void *high (void *n)
{
	int now ;
	struct sched_param the_priority;

	the_priority.sched_priority  = 50;
	pthread_setschedparam(pthread_self(), SCHED_FIFO, &the_priority);
	sem_post(&high_ready);
	sem_wait(&high_go);
	now=gettime();
	pthread_mutex_lock(&shared_mutex);
	pthread_mutex_unlock(&shared_mutex);
	printf ("high took %d seconds wanted about %d (low critical section)\n",gettime() - now,LOW_SPIN);
	return NULL;
}

int main ()
{
	pthread_t tid1, tid2, tid3;
	cpu_set_t thecpus;


	CPU_ZERO(&thecpus);
	CPU_SET(0, &thecpus);
	if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &thecpus)< 0)
		perror("set affinity");

	if (pthread_mutexattr_init(&mutex_attr)) 
	{perror("mutex init"); exit(1);}

#if defined(_POSIX_THREAD_PRIO_INHERIT) && _POSIX_THREAD_PRIO_INHERIT != -1 && defined(USE_PRI_INHERIT)
	printf("Using priority inheritance\n");
	if (pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT))
	{perror("mutex init"); exit(1);}
#else
	printf("Not Using priority inheritance\n");

#endif

	if (pthread_mutex_init(&shared_mutex, &mutex_attr))
	{perror("mutex init"); exit(1);}
	// all initialized to zero. Must wait on a post
	sem_init (&low_go,0,0);
	sem_init (&mid_go,0,0);
	sem_init (&high_go,0,0);
	sem_init (&high_ready,0,0); 
	sem_init (&mid_ready,0,0); 

	pthread_create(&tid1, NULL, low, NULL);
	pthread_create(&tid2, NULL, mid, NULL);
	pthread_create(&tid3, NULL, high, NULL);

	sem_post(&low_go);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);

	return 0;
}
