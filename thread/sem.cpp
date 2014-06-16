#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>

using   namespace   std;

#define NBUFF 100
int nitems;
sem_t mutex;
sem_t nempty;
sem_t nstored;
int buff[NBUFF];
void *produce(void *arg)
{
   int i;
   for (i = 0; i < nitems; i++) {
     sem_wait(&nempty);
     sem_wait(&mutex);
     buff[i % NBUFF] = i;
     cout << "Product " << buff[i % NBUFF] << endl;
     sem_post(&mutex);
     sem_post(&nstored);
   }
   return(NULL);
}
void *consume(void *arg)
{
   int i;
   for (i = 0; i < nitems; i++) {
     sem_wait(&nstored);
     sem_wait(&mutex);
     if (buff[i % NBUFF] != i)
        cout << "buff[" << i  <<"] = " <<  buff[i % NBUFF] << endl;
     cout << "Consumer:" << buff[i % NBUFF] << endl;
     sem_post(&mutex);
     sem_post(&nempty);
   }
   return(NULL);
}
int main(int argc, char **argv)
{
   pthread_t tid_produce, tid_consumer;
   if (argc != 2){      cout << "Usage: prodcons number" << endl; exit(0);   }
   nitems = atoi(argv[1]);
   sem_init(&mutex, 0, 1);
   sem_init(&nempty, 0, NBUFF);
   sem_init(&nstored, 0, 0);
   pthread_create(&tid_produce, NULL, produce, NULL);
   pthread_create(&tid_consumer, NULL, consume, NULL);
   pthread_join(tid_produce, NULL);
   pthread_join(tid_consumer, NULL);
   sem_destroy(&mutex);
   sem_destroy(&nempty);
   sem_destroy(&nstored);
   exit(0);
}
