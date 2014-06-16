#include <stdio.h>
#include <sched.h>
#include <stdlib.h>

int data = 10;

int child_process()
{
	printf("Child process %d, data %d\n",getpid(),data);
	data = 20;
	printf("Child process %d, data %d\n",getpid(),data);
	while(1);
}

int main(int argc, char* argv[])
{
	void **child_stack;
	child_stack = (void **) malloc(16384);
	clone(child_process, child_stack + 16384, CLONE_VM|CLONE_FILES|CLONE_SIGHAND, NULL);

	sleep(1);
	printf("Parent process %d, data %d\n",getpid(), data);
	while(1);
}
