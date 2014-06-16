#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
typedef struct{
	char name[4];
	int age;
} people;
main(int argc, char** argv)
{
	int shm_id,i;
	key_t key;
	char temp;
	people *p_map;
	char* name = "/dev/shm/myshm2";
	key = ftok(name,0);
	if(key==-1) {
		perror("ftok error");
		exit(-1);
	}
	shm_id=shmget(key,4096,IPC_CREAT|0666);
	printf("shm_id:%d\n",shm_id);
	if(shm_id==-1)
	{
		perror("shmget error");
		return;
	}

	p_map=(people*)shmat(shm_id,NULL,0);
	temp='a';
	printf("%08x\n", p_map);
	for(i = 0;i<10;i++)
	{
		temp+=1;
		memcpy((*(p_map+i)).name,&temp,1);
		(*(p_map+i)).age=20+i;
	}
	if(shmdt(p_map)==-1)
		perror(" detach error ");
	sleep(100);
	system("ipcs");
	shmctl(shm_id, IPC_RMID, NULL) ;

	printf("--------------------------------------------------\n");
	system("ipcs");
}

