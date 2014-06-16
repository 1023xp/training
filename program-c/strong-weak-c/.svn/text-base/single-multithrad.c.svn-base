#include <stdio.h>
#include <pthread.h>
int pthread_create( pthread_t*, const pthread_attr_t*,
void* (*)(void*), void*) __attribute__ ((weak));
int main()
{
    if(pthread_create)
    {
            printf("This is multi-thread version!\n");
            // run the multi-thread version
            // main_multi_thread()
    }
    else
    {
            printf("This is single-thread version!\n");  
            // run the single-thread version
            // main_single_thread()
    }
}

