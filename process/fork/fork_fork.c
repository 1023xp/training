#include <stdio.h>
#include <sched.h>
#include <signal.h>

void sigint_handle(int sig)
{
	printf("got the %d signal...\n", sig);
	exit(0);
}

int main(int argc, char* argv[])
{
	if(signal(SIGINT, sigint_handle) == SIG_ERR){
		perror("cannot reset the SIGINT signal handler");
		return 1;
	}

	fork();
	fork();
	printf("pid:%d hello world\n", getpid());
	while(1);
}
