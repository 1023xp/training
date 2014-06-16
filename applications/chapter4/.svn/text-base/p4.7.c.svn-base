#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char* argv[]){
	if(argc!=2){
		printf("Usage:%s filename\n",argv[0]);
		return 1;
	}

	if(chmod(argv[1],S_IRUSR | S_IRGRP | S_IXOTH | S_IROTH)<0){
		perror("Cannot modify the Permission of the file");
		return 1;
	}

	return 0;
}
