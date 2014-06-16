#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void){
	pid_t pid;

	if((pid=fork())<0){
		perror("Cannot create the new process");
		return 1;
	}else if(pid==0){
		printf("In the child process!\n");
		return 0;
	}else {
		printf("In the parent process!\n");
		return 0;
	}
}
