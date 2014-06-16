#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>

int main(int argc,char *argv[])
{
	int srv_sock;
	socklen_t clt_len;
	struct sockaddr_in srv_addr;
	struct sockaddr_in clt_addr;
	struct timeval wait_time;
	int port;
	int ret;
	fd_set read_fds;
	int num;	
	char recv_buf[1024];

	if(argc!=2){
		printf("Usage: %s port_name\n",argv[0]);
		return 1;
	}
	
	port=atoi(argv[1]);

	srv_sock=socket(PF_INET,SOCK_DGRAM,0);
	if(srv_sock<0){
		perror("cannot create socket");
		return 1;
	}

	memset(&srv_addr,0,sizeof(srv_addr));
	srv_addr.sin_family=AF_INET;
	srv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	srv_addr.sin_port=htons(port);

	ret=bind(srv_sock,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
	if(ret<0){
		perror("cannot bind the socket");
		return 1;
	}

	while(1){
		wait_time.tv_sec=3;
		wait_time.tv_usec=0;
		FD_ZERO(&read_fds);
		FD_SET(srv_sock,&read_fds);
		
		num=select(srv_sock+1,&read_fds,NULL,NULL,&wait_time);
		if(num<0){
			perror("select fail");
			continue;
		}
		
		if(FD_ISSET(srv_sock,&read_fds)){
			int n=recvfrom(srv_sock,recv_buf,sizeof(recv_buf),0,(struct sockaddr*)&clt_addr,&clt_len);
			if(n<0){
				perror("cannot receive client message");
				close(srv_sock);
				return 1;
			}

			printf("server receive: %s\n",recv_buf);
			memset(recv_buf,0,sizeof(recv_buf));
		}
		printf("waiting....\n");
	}
}
