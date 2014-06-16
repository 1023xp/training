#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char* argv[]){
	int fd;
	char buffer[100];
	int num;

	if(argc!=2){
		printf("Usage : %s filename",argv[0]);
		return 1;
	}

	if((fd=open(argv[1],O_RDONLY))==-1){
		perror("Cannot open the file");
		return 1;
	}

        while((num=read(fd,buffer,100))>0){
		printf("%s",buffer);
	}

  	close(fd);
	return 0;
}
