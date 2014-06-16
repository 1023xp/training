#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc,char* argv[])
{
	int fd;
	int count;
	count=0;

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

	lock.l_type=F_WRLCK;
	lock.l_whence=0;
	lock.l_start=0;
	lock.l_len=0;

	while(fcntl(fd,F_SETLK,&lock)<0){
		switch(errno){
			case EAGAIN:
			case EACCES:
				if(++count<5)
					sleep(1);
				else{
					fcntl(fd,F_GETLK,&lock);
					printf("pid: %ld process find pid: %ld lock the file %s\n",(long)getpid(),(long)lock.l_pid,argv[1]);
					return 1;
				}
				continue;
		}
		perror("function fcntl call fail");
		return 1;
	}
	
	printf("pid: %ld process lock the file\n",(long)getpid());
	sleep(1);
	printf("pid: %ld process release the file\n",(long)getpid());

	return 0;
}
