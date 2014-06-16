#include <stdio.h>
#include <unistd.h>

int main(int argc,char* argv[])
{
	int f_des[2];
	int pid;
	char msg[BUFSIZ];
	
	if(argc!=2){
		printf("Usage: %s message\n",argv[0]);
		return 1;
	}

	if(pipe(f_des)==-1){
		perror("cannot create the IPC pipe");
		return 1;
	}

	pid=fork();
	if(pid==-1){
		perror("cannot create new process");
		return 1;
	}else if(pid==0){
		close(f_des[1]);

		if(read(f_des[0],msg,BUFSIZ)==-1){
			perror("child process cannot read data from pipe");
			return 1;
		}else
			printf("in child process, receive message: %s\n",msg);
		
		_exit(0);
	}else {
		close(f_des[0]);

		if(write(f_des[1],argv[1],strlen(argv[1]))==-1){
			perror("parent process cannot write data to pipe");
			return 1;
		}else
			printf("in parent process, send message: %s\n",argv[1]);
		
		wait(NULL);
		_exit(0);
	}

	return 0;
}
