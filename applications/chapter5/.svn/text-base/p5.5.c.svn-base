#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char* argv[]){
	int len;
	int fd;
	long offset=0;
	char buffer[1024];
	int flag=0;

	if(argc!=3){
		printf("Usage: %s \"string\" filename\n",argv[0]);
		return 1;
	}

	len=strlen(argv[1]);


	if((fd=open(argv[2],O_RDONLY))==-1){
		perror("Cannot open the desired file");
		return 1;
	}


	while(1){
		if(lseek(fd,offset,SEEK_SET)==-1){
			perror("Cannot move the file pointer");
			return 1;
		}

		if(read(fd,buffer,len)<len)
			break;
		
		buffer[len]='\0';
		
		if(strcmp(buffer,argv[1])==0)
			flag++;

		offset++;
	}

	if(flag>0)
		printf("Find the string: %s in the file: %s %d times\n",argv[1],argv[2],flag);

	close(fd);

	return 0;
}
