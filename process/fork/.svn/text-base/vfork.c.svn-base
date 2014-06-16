#include <stdio.h>
#include <sched.h>

int data = 10;

int child_process()
{
	printf("Child process %d, data %d\n",getpid(),data);
	data = 20;
	printf("Child process %d, data %d\n",getpid(),data);
//	while(1);
}

int main(int argc, char* argv[])
{
	if(vfork()==0) {
		child_process();	
	}
	else{
		sleep(1);
		printf("Parent process %d, data %d\n",getpid(), data);
		while(1);
	}
}
