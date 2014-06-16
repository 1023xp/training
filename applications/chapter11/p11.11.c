#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

extern int etext,edata,end;

int main(int argc,char* argv[])
{
	int shmid;
	int proj_id;
	key_t key;
	int shm_size;
	char* shm_addr,*addr;
	pid_t pid;
	
	if(argc!=3){
		printf("Usage: %s shared_memory_size info\n",argv[0]);
		return 1;
	}

	shm_size=atoi(argv[1]);

	proj_id=2;
        key=ftok("/home/program",proj_id);
        if(key==-1){
                perror("cannot generate the IPC key");
                return 1;
        }

	shmid=shmget(key,shm_size,IPC_CREAT | 0660);
	if(shmid==-1){
		perror("cannot create a shared memory segment");
		return 1;
	}

	addr=(char*)shmat(shmid,NULL,0);
	shm_addr=addr;
	if(shm_addr==(char*)(-1)){
		perror("cannot attach the shared memory to process");
		return 1;
	}

	printf("====address information===\n");
	printf("etext address: %x\n",&etext);
	printf("edata address: %x\n",&edata);
	printf("end address: %x\n",&end);
	printf("shared memory segment address: %x\n",shm_addr);
	printf("==========================\n");
	
	strcpy(shm_addr,argv[2]);

	printf("the input string is : %s\n",argv[2]);
	
	printf("before fork,in shared memory segment the string is: %s\n",shm_addr);
	pid=fork();
	if(pid==-1){
		perror("cannot create new process");
		return 1;
	}else if(pid==0){
		printf("in child process, the string is %s \n",shm_addr);
		printf("modify the content in shared memory\n");
		
		*shm_addr +=1;	
		
		_exit(0);			
	}else{
		wait(NULL);

		printf("after fork, in shared memory segment the string is %s\n",shm_addr);
		if(shmdt(shm_addr)==-1){
			perror("cannot release the memory");
			return 1;
		}
		
		if(shmctl(shmid,IPC_RMID,NULL)==-1){
			perror("cannot delete existing shared memory segment");
			return 1;
		}
	}

	return 0;
}
