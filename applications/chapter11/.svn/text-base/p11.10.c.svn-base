#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define LINUX_ENV

#ifdef LINUX_ENV
#define _GNU_SOURCE
#endif

int main(void)
{
	key_t key;
	int proj_id;
	int shmid;

	struct shmid_ds buffer;

	proj_id=2;
	key=ftok("/home/program",proj_id);
	if(key==-1){
		perror("cannot generate the IPC key");
		return 1;
	}

	shmid=shmget(key,1024,IPC_CREAT | 0660);
	if(shmid==-1){
		perror("cannot create a shared memory segment");
		return 1;
	}

	shmctl(shmid,IPC_STAT,&buffer);

	printf("=======shared memory info========\n");
	printf("effective user id : %d \n",buffer.shm_perm.uid);
	printf("effective group id: %d \n",buffer.shm_perm.gid);
	printf("message queue's creator user id : %d \n",buffer.shm_perm.cuid);
	printf("message queue's creator group id : %d \n",buffer.shm_perm.cgid);
	printf("access mode : %x\n",buffer.shm_perm.mode);
       
	printf("PID of creator : %d \n",buffer.shm_cpid);
	printf("No. of current attaches: %d \n",buffer.shm_nattch);

	//special parameter, only for linux
#ifdef LINUX_ENV
	shmctl(shmid,SHM_INFO,&buffer);

	struct shm_info* mem_info;
	mem_info=(struct shm_info *)(&buffer);

	printf("# of currently existing segments: %d \n",mem_info->used_ids);
	printf("Total number of shared memory pages : %d \n",mem_info->shm_tot);
	printf("# of resident shared memory pages: %d \n",mem_info->shm_rss);
	printf("# of swapped shared memory pages: %d \n",mem_info->shm_swp);
#endif
}
