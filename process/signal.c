#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigusr1(int sig)
{
	printf("got the SIGUSR1 signal...\n");
//	exit(0);
}

int main(void)
{
	/* replace 2 by 9, and prove 9 can't be ignored and blocked */
	if(signal(9,SIG_IGN/*sigusr1*/)==SIG_ERR){
		perror("cannot reset the SIGINT signal handler");
		return 1;
	}

	if(signal(SIGUSR1,sigusr1)==SIG_ERR){
		perror("cannot reset the SIGUSR1 signal handler");
		return 1;
	}

	while(1);

	return 0;
}
