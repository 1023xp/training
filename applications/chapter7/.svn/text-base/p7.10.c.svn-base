#include <stdio.h>
#include <unistd.h>

int main(int argc,char* argv[]){
	if(argc<2){
		printf("Usage:  %s path\n",argv[0]);
		return 1;	
	}

	execlp("/bin/ls","ls",argv[1],(char*)NULL);
	return 0;
}
