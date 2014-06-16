#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc,char* argv[])
{
	int fd;
	int lock_result;

	struct flock lock;
	
	if(argc!=2){
		printf("Usage: %s lock_file_name\n",argv[0]);
		return 1;
	}	

	fd=open(argv[1],O_RDWR);
	if(fd<0){
		perror("cannot open the file");
		return 1;
	}

	lock_result=lockf(fd,F_LOCK,0);
	
	if(lock_result==-1){
		perror("cannot get the lock....");
		return 0;
	}


	printf("pid: %ld process lock the file\n",(long)getpid());
	sleep(1);
	printf("pid: %ld process release the file\n",(long)getpid());

	return 0;
}
