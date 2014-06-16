#include <stdio.h>
#include <unistd.h>

int main(int argc,char* argv[]){
	if(argc<2){
		printf("Usage: %s arg list...\n",argv[0]);
		return 1;
	}

	execvp(argv[1],&argv[1]);
	return 0;
}
