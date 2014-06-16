#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


int  get_access_right(int inq_interval, int retry_times, char* file_name)
{
	int fd,count;
	count=0;

	fd=creat(file_name,0);
	while(fd==-1 && errno==EACCES){
		printf("pid:%ld process try to get access right...\n",(long)getpid());

		if(++count<retry_times)
			sleep(inq_interval);
		else
			return -1;
		fd=creat(file_name,0);
	}

	close(fd);

	if(fd!=-1)
		return 1;
	else 
		return -1;
}

int  release_right(char* file_name)
{
	int result;

	result=unlink(file_name);

	if(result==0)
		return 1;
	else
		return -1;
}

int main(int argc,char* argv[])
{
	int retry_times,inq_interval;
	char* file_name;
	int len;
	int count;
	count=0;

	if(argc!=4){
		printf("Usage: %s retry_times inquire_interval file_name\n",argv[0]);
		return 1;
	}

	retry_times=atoi(argv[1]);
	if(retry_times<=0){
		printf("illegal retry times!\n");
		return 1;
	}

	inq_interval=atoi(argv[2]);
	if(inq_interval<=0){
		printf("illegal inquiring interval\n");
		return 1;
	}

	len=strlen(argv[3]);
	if(len==0){
		printf("illegal file name\n");
		return 1;
	}


	if(get_access_right(inq_interval,retry_times,argv[3])==1){
		while(count<5){
			printf("pid: %ld  process is occupying the resource, circle %d \n",(long)getpid(),count);
			count++;
			sleep(inq_interval);
		}

		release_right(file_name);
		return 0;
	}else
		printf("pid: %ld  process cannot access the resource...retry %d times\n",(long)getpid(),retry_times);

	return 0;
}
