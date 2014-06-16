#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
	printf("Current Process ID :%ld\n",(long)getpid());
	printf("The parent process ID of the Current process :%ld\n",getppid());

	return 0;
}
