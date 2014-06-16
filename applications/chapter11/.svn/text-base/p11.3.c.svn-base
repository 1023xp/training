#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define LINUX_ENV

#ifdef LINUX_ENV
#define _GNU_SOURCE
#endif

int main(void)
{
	key_t key;
	int proj_id;
	int msqid;

	struct msqid_ds buffer;

	proj_id=2;
	key=ftok("/home/program",proj_id);
	if(key==-1){
		perror("cannot generate the IPC key");
		return 1;
	}

	msqid=msgget(key,IPC_CREAT | 0660);
	if(msqid==-1){
		perror("cannot create message queue resource");
		return 1;
	}

	msgctl(msqid,IPC_STAT,&buffer);

	printf("=======Message Queue Info========\n");
	printf("effective user id : %d \n",buffer.msg_perm.uid);
	printf("effective group id: %d \n",buffer.msg_perm.gid);
	printf("message queue's creator user id : %d \n",buffer.msg_perm.cuid);
	printf("message queue's creator group id : %d \n",buffer.msg_perm.cgid);
	printf("access mode : %x\n",buffer.msg_perm.mode);
       
	printf("Maximum number of bytes allowed in message queue : %d \n",buffer.msg_qbytes);
	printf("Current number of bytes in message queue (non-standard) : %d \n",buffer.__msg_cbytes);
	printf("Current number of messages in message queue : %d \n",buffer.msg_qnum);

	//special parameter, only for linux
#ifdef LINUX_ENV
	msgctl(msqid,MSG_INFO,&buffer);

	struct msginfo* queue_info;
	queue_info=(struct msginfo *)(&buffer);

	printf("Size in bytes of buffer pool used to hold message data : %d \n",queue_info->msgpool);
	printf("Max. # of entries in message map : %d \n",queue_info->msgmap);
	printf("Max. # of bytes that can be written in a single message : %d \n",queue_info->msgmax);
	printf("Max. # of bytes that can be written to queue : %d \n",queue_info->msgmnb);
	printf("Max. # of message queues : %d \n",queue_info->msgmni);
	printf("Max. # of segments : %d \n",queue_info->msgseg);
#endif
}
