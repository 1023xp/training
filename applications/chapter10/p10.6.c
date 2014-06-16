#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define FIFO_CHANNEL "/tmp/fifo.channel"

int main(int argc,char* argv[])
{
	int fd;
	int pid;
	char r_msg[BUFSIZ];
	char* s_msg="send message to other process";
	
	if(argc!=2){
		printf("Usage: %s opt(read-2/write-1)\n",argv[0]);
		return 1;
	}

	//write the message to the FIFO
	if(atoi(argv[1])==1){
		if(mkfifo(FIFO_CHANNEL,0777)==-1){
			perror("cannot create FIFO channel");
			return 1;
		}
		
		fd=open(FIFO_CHANNEL,O_WRONLY);
		if(fd==-1){
			perror("cannot open the FIFO");
			return 1;
		}
	
		if(write(fd,s_msg,strlen(s_msg))==-1){
			perror("process cannot write data to FIFO");
			return 1;
		}else
			printf("Send message: %s\n",s_msg);
	}

	//receive the message from the FIFO
	if(atoi(argv[1])==2){
		fd=open(FIFO_CHANNEL,O_RDONLY);
                if(fd==-1){
                        perror("cannot open the FIFO");
                        return 1;
                }
		
		if(read(fd,r_msg,BUFSIZ)==-1){
			perror("process cannot read data from FIFO");
			return 1;
		}else
			printf("Receive message from FIFO: %s\n",r_msg);
	}

	return 0;
}
