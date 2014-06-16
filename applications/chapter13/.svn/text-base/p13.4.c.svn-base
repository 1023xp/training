#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <netdb.h>

void sig_handler(int signo)
{
	pid_t pid;
	int stat;

	pid=waitpid(-1,&stat,WNOHANG);
	while(pid>0){
		printf("child process terminated (PID: %ld)\n",(long)getpid());
		pid=waitpid(-1,&stat,WNOHANG);
	}

	return;
}

int main(int argc,char *argv[])
{
	socklen_t clt_addr_len;
	int listen_fd;
	int com_fd;
	int ret;
	int i;
	static char recv_buf[1024];	
	int len;
	int port;
	pid_t pid;

	struct sockaddr_in clt_addr;
	struct sockaddr_in srv_addr;

	if(argc!=2){
		printf("Usage: %s port\n",argv[0]);
		return 1;
	}

	port=atoi(argv[1]);

	if(signal(SIGCHLD,sig_handler)<0){
		perror("cannot set the signal");
		return 1;
	}

	listen_fd=socket(PF_INET,SOCK_STREAM,0);
	if(listen_fd<0){
		perror("cannot create listening socket");
		return 1;
	}

	memset(&srv_addr,0,sizeof(srv_addr));
	srv_addr.sin_family=AF_INET;
	srv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	srv_addr.sin_port=htons(port);

	ret=bind(listen_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
	if(ret==-1){
		perror("cannot bind server socket");
		close(listen_fd);
		return 1;
	}

	ret=listen(listen_fd,5);
	if(ret==-1){
		perror("cannot listen the client connect request");
		close(listen_fd);
		return 1;
	}
	
	while(1){
		len=sizeof(clt_addr);
		com_fd=accept(listen_fd,(struct sockaddr*)&clt_addr,&len);
		if(com_fd<0){
			if(errno==EINTR){
				continue;
			}else{
				perror("cannot accept client connect request");
				close(listen_fd);
				return 1;
			}
		}

		pid=fork();
		if(pid<0){
			perror("cannot create the child process");
			close(listen_fd);
			return 1;
		}else if(pid==0){
			while((len=read(com_fd,recv_buf,1024))>0){
				printf("Message from client(%d): %s\n",len,recv_buf);
				if(recv_buf[0]=='@')
					break;
				for(i=0;i<len;i++)
					recv_buf[i]=toupper(recv_buf[i]);
				write(com_fd,recv_buf,len);
			}

		close(com_fd);
		return 0;
		}else
			close(com_fd);
	}	
	
	return 0;
}
