#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct {
        long mtype;
        char mtext[BUFSIZ];
}msg_info;

int recv_msg(int msqid,int msg_type,char* msg)
{
	int result;
	msg_info buffer;

	result=msgrcv(msqid,&buffer,BUFSIZ,msg_type,0);
	if(result==-1)
		perror("cannot receive message from the queue");

	strcpy(msg,buffer.mtext);	

	return result;
}

int main(int argc,char* argv[])
{
	char buf[BUFSIZ];
	int msqid;
	int msg_type;
	int result;

	if(argc!=3){
		printf("Usage: %s msqid message_type\n");
		return -1;
	}
	
	msqid=atoi(argv[1]);
	msg_type=atoi(argv[2]);
	
	result=recv_msg(msqid,msg_type,buf);
	if(result==-1){
		printf("cannot get message");
		return -1;
	}else
		printf("message queue id: %d message type : %d message : %s\n",msqid,msg_type,buf);

	return 0;
}
