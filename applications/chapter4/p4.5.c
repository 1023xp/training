#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void){
	char test;
	int fd;

	if((fd=open("test.dat",O_RDONLY))==-1){
	    perror("Cannot open the test.dat file");
	    return 1;
	}

	if(fork()==-1){
	    perror("Cannot create the child process");
	    return 1;
	}

	read(fd,&test,1);
	printf("Process ID: %ld read the character: %c\n",(long)getpid(),test);
        close(fd);
	return 0;
}

