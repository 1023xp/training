#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char* argv[]){
	int fd1,fd2;
	char buffer[100];
	int num;

	if(argc!=3){
		printf("Usage : %s source dest",argv[0]);
		return 1;
	}

	if((fd1=open(argv[1],O_RDONLY))==-1){
		perror("Cannot open the file");
		return 1;
	}

	if((fd2=open(argv[2],O_CREAT | O_WRONLY,0777))==-1){
		perror("Cannot create the destination file");
		return 1;
	}

	

        while((num=read(fd1,buffer,100))>0){
		
		if(write(fd2,buffer,num)==-1){
			perror("Cannot write the file content to the file");
			return 1;
		}

	}
	
	close(fd1);
	close(fd2);
	return 0;
}
