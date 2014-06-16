#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char* argv[]){
	pid_t pid,wait_pid;
	int status;
	int i;

	if(argc<4){
		printf("Usage: %s para1 para2 para3\n",argv[0]);
		return 1;
	}

	for(i=1;i<4;i++)
		if((pid=fork())==0)
			execl("./p7.14","p7.14",argv[i],NULL);
		else
			printf("create the child process id: %d\n",pid);

	while((wait_pid=wait(&status)) && wait_pid!=-1)
		printf("process id:%d exit,exit_code is %0x\n",wait_pid,status);
	
	return 0;
}
