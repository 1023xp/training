#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc,char* argv[]){
	pid_t pid;
	int exit_code;

	pid=getpid();

	srand((unsigned)pid);
	exit_code=(int)(rand() % 256);

	sleep(10);

	if(atoi(*(argv+1)) % 2){
		printf("the child process id:%d receive signal SIGKILL\n",pid);
		kill(pid,9);
	}else{
		printf("the child process id:%d normally exit,exit_code is %0x\n",pid,exit_code);
		exit(exit_code);
	}
}
