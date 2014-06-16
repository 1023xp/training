#include <stdio.h>
#include <unistd.h>

int main(int argc,char* argv[])
{
	int f_des[2];
	int pid;
	
	if(argc!=3){
		printf("Usage: %s comand1 comand2\n",argv[0]);
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
		dup2(f_des[1],STDOUT_FILENO);
		
		close(f_des[0]);
		close(f_des[1]);

		if(execlp(argv[1],argv[1],NULL)==-1){
			perror("in child process,cannot execute the command");
			return 1;
		}
		
		return 1;
	}else {
		dup2(f_des[0],STDIN_FILENO);

		close(f_des[0]);
		close(f_des[1]);
	
		if(execlp(argv[2],argv[2],NULL)==-1){
			perror("in parent process,cannot execute the command");
			return 1;
		}
			
		return 1;
	}

	return 0;
}
