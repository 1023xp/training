#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define DUP_STDIO

static void zap_stdio(void)
{
	int fd;
	fd = open("/dev/null", O_RDWR);
	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);
	close(fd);
}

int main(int argc,char* argv[]){
	if(argc<2){
		printf("Usage:  %s path\n",argv[0]);
		return 1;	
	}
#ifdef DUP_STDIO
	zap_stdio();
#endif
	execlp("/bin/ls","ls",argv[1],(char*)NULL);
	return 0;
}
