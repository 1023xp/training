#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char* argv[])
{
	int fd;
	int file_stat;

	if(argc!=2){
		printf("Usage: %s filename\n",argv[0]);
		return 1;
	}

	fd=open(argv[1],O_RDONLY);
	if(fd==-1){
		perror("Cannot open the file");
		return 1;
	}

	file_stat=fcntl(fd,F_GETFL);
	if(file_stat<0){
		perror("Cannot get the file status");
		return 1;
	}
	
	switch(file_stat & O_ACCMODE){
		case O_RDONLY:
			printf("file:%s read only\n",argv[0]);
			break;
		case O_WRONLY:
			printf("file:%s write only\n",argv[0]);
			break;
		case O_RDWR:
			printf("file:%s read and write\n",argv[0]);
			break;
		default:
			printf("unkown access right\n");
	}

	return 0;
}

