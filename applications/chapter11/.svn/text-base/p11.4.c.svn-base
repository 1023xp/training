#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct {
	long mtype;
	char mtext[BUFSIZ];
}msg_info;

int creat_msg_queue()
{
        key_t key;
        int proj_id;
        int msqid;

        struct msqid_ds buffer;

        proj_id=2;
        key=ftok("/home/program",proj_id);
        if(key==-1){
                perror("cannot generate the IPC key");
                return -1;
        }

        msqid=msgget(key,IPC_CREAT | 0660);
        if(msqid==-1){
                perror("cannot create message queue resource");
                return -1;
        }

	return msqid;
}


int send_msg(int msqid,char* msg)
{
	int result;
	
	msg_info buf;
	
	buf.mtype=100;
	strcpy(buf.mtext,msg);
	
	result=msgsnd(msqid,&buf,strlen(msg),0);
	if(result==-1)
		perror("cannot send message to the message queue");
	
	return result;
}

int show_msg_queue_stat(int msqid)
{
	struct msqid_ds buffer;
	int flg;

	flg=msgctl(msqid,IPC_STAT,&buffer);
	if(flg==-1){
		perror("cannot get status of the message queue");
		return -1;
	}	

        printf("=======Message Queue Info========\n");
        printf("effective user id : %d \n",buffer.msg_perm.uid);
        printf("effective group id: %d \n",buffer.msg_perm.gid);
        printf("message queue's creator user id : %d \n",buffer.msg_perm.cuid);
        printf("message queue's creator group id : %d \n",buffer.msg_perm.cgid);
        printf("access mode : %x\n",buffer.msg_perm.mode);

        printf("Maximum number of bytes allowed in message queue : %d \n",buffer.msg_qbytes);
        printf("Current number of bytes in message queue (non-standard) : %d \n",buffer.__msg_cbytes);
        printf("Current number of messages in message queue : %d \n",buffer.msg_qnum);

	return 0;
}

int main(int argc,char* argv[])
{	
	int msqid;

	if(msqid=creat_msg_queue()<0){
		printf("create msg queue fail");
		return -1;
	}

	if(send_msg(msqid,"test message")<0){
		printf("send message fail");
		return -1;
	}	

	if(show_msg_queue_stat(msqid)<0){
		printf("show message queue fail");
		return -1;
	}

	return 0;
}	
