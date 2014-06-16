#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void){
	int fd1;

	if((fd1=open("test1",O_CREAT | O_RDWR,0777))==-1){
	    perror("Cannot create the test2 file");
	    return 1;
	}

	close(fd1);

	return 0;
}

