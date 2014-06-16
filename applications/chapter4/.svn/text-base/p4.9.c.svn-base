#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main(void){
	DIR* dir;
	struct dirent* drt;
	int i;

	dir=opendir("/etc/pcmcia");
	if(dir==NULL){
		perror("Cannot open the desired directory");
		return 1;
	}

	while((drt=readdir(dir))!=NULL){
		printf("filename or directory : %s\n",drt->d_name);
	}
	
	closedir(dir);
	return 0;
}

