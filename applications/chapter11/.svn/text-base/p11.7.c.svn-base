#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define LINUX_ENV

#ifdef LINUX_ENV
#define _GNU_SOURCE
#endif

union semun{
	int val;
	struct semid_ds * buf;
	unsigned short int * array;
	struct seminfo * __buf;
};

int main(void)
{
	key_t key;
	int proj_id;
	int semid;
	int nsems;
	union semun arg;
	struct semid_ds buffer;

	proj_id=2;
	key=ftok("/home/program",proj_id);
	if(key==-1){
		perror("cannot generate the IPC key");
		return 1;
	}
	
	nsems=2;
	semid=semget(key,nsems,IPC_CREAT | 0660);
	if(semid==-1){
		perror("cannot create semaphore set resource");
		return 1;
	}
	
	arg.buf=&buffer;
	if(semctl(semid,0,IPC_STAT,arg)==-1){
		perror("cannot get semaphore set info");
		return 1;
	}

	printf("=======semphore set info========\n");
	printf("effective user id : %d \n",buffer.sem_perm.uid);
	printf("effective group id: %d \n",buffer.sem_perm.gid);
	printf("semaphore set's creator user id : %d \n",buffer.sem_perm.cuid);
	printf("semaphore set's creator group id : %d \n",buffer.sem_perm.cgid);
	printf("access mode : %x\n",buffer.sem_perm.mode);
       
	//special parameter, only for linux
#ifdef LINUX_ENV
	if(semctl(semid,0,SEM_INFO,arg)==-1){
		perror("cannot get semphore set info");
		return 1;
	}

	struct seminfo* sem_info;
	sem_info=(struct seminfo *)(&buffer);

	printf("Max. # of semaphore sets: %d \n",sem_info->semmni);
	printf("Max. # of semaphores in all semaphore sets: %d \n",sem_info->semmns);
	printf("Max. # of semaphores in a set: %d \n",sem_info->semmsl);
	printf("Max. # of operations for semop(): %d \n",sem_info->semopm);
	printf("size of struct sem_undo: %d \n",sem_info->semusz);
	printf("Max. value that can be recorded for semaphore adjustment (SEM_UNDO): %d \n",sem_info->semaem);
#endif
}
