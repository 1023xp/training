#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int g_var=0;

int main(void){
	pid_t pid;
	int var=1;

	printf("process id:%ld\n",(long)getpid());
	printf("before execute the fork system call, g_var=%d var=%d\n",g_var,var);

	if((pid=vfork())<0){
		perror("Cannot create a new process");
		return 1;
	}else if(pid==0){
		g_var++;
		var++;
		printf("process id : %ld, g_var=%d var=%d\n",(long)getpid(),g_var,var);
		//_exit(0);
		//return 0;
		exit(0);
	}

	printf("process id : %ld, g_var=%d var=%d\n",(long)getpid(),g_var,var);
	return 0;
}
