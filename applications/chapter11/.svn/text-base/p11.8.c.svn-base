#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun{
        int val;
        struct semid_ds * buf;
        unsigned short int * array;
        struct seminfo * __buf;
};

int main(int argc,char* argv[])
{
	int semid;
	pid_t pid;
	int proj_id;
	key_t key;	
	int num;
	int i,j;

        union semun arg;
	
	static struct sembuf acquire={0,-1,SEM_UNDO};
	static struct sembuf release={0,1,SEM_UNDO};

        if(argc!=2){
		printf("Usage : %s num\n",argv[0]);
		return -1;
	}
	
	num=atoi(argv[1]);

	proj_id=2;
        key=ftok("/home/program",proj_id);
        if(key==-1){
                perror("cannot generate the IPC key");
                return -1;
        }

	semid=semget(key,1,IPC_CREAT | IPC_EXCL | 0666);
	if(semid==-1){
		perror("cannot create semaphore set");
		return -1;
	}	

	static unsigned short start_var=1;
	arg.array=1;
	if(semctl(semid,0,SETVAL,arg)==-1){
		perror("cannot set semaphore set");
		return -1;	
	}	

	for(i=0;i<num;i++){
		pid=fork();
		
		if(pid<0){
			perror("cannot create new process");
			return -1;
		}else if(pid==0){
			semid=semget(key,1,0);
			if(semid==-1){
				perror("cannot let the process get the access right");
				_exit(-1);
			}	

			for(j=0;j<2;j++){
				sleep(i);
				if(semop(semid,&acquire,1)==-1){
					perror("cannot acquire the resource");
					_exit(-1);
				}

				printf("====enter the critical section=====\n");
				printf("---pid : % ld ---\n",(long)getpid());
				sleep(1);
				printf("====leave the critical section=====\n");

				if(semop(semid,&release,1)==-1){
					perror("cannot release the resource");
					_exit(-1);
				}
			}
			_exit(0);
		}
	}

	for(i=0;i<num;i++)
		wait(NULL);

	if(semctl(semid,0,IPC_RMID,0)==-1){
		perror("cannot remove the semaphore set");
		return -1;
	}

	return 0;
}
