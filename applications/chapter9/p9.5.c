#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	int pid;
	int wait_pid;
	int status;
	int i;

	for(i=1;i<=5;i++){

		pid=fork();

		if(pid<0){
			perror("cannot create the new process");
			return 1;
		}else if(pid==0){
			printf("in child process(pid: %ld ), the process will terminate in %d seconds\n",(long)getpid(),i);
			alarm(i);
			pause();
		}
	}

       	while((wait_pid=wait(&status)) && wait_pid!=-1){
		//如果进程是由于接收到退出信号而结束
		if(WIFSIGNALED(status))
			printf("process id:%d Receive SIG :%d exit\n",pid,WTERMSIG(status));
									
		//如果进程是正常退出的话
		if(WIFEXITED (status))
			printf("process id:%d exit code %d\n",pid,WEXITSTATUS(status));
	}

	return 0;
}
