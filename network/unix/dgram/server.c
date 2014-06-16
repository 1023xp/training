#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define UNIX_DOMAIN "unix.domain"

int main()
{
	socklen_t addr_len;
	int listen_fd;
	int com_fd;
	int ret;
	int i;
	static char recv_buf[1024];	
	int len;

	struct sockaddr_un clt_addr;
	struct sockaddr_un srv_addr;

	com_fd=socket(PF_UNIX,SOCK_DGRAM,0);
	if(com_fd<0){
		perror("cannot create listening socket");
		return 1;
	}

	srv_addr.sun_family=AF_UNIX;
	strcpy(srv_addr.sun_path,UNIX_DOMAIN);
	unlink(UNIX_DOMAIN);

	ret=bind(com_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr.sun_family)+strlen(srv_addr.sun_path));
	if(ret==-1){
		perror("cannot bind server socket");
		close(com_fd);
		unlink(UNIX_DOMAIN);
		return 1;
	}

	for(i=0;i<4;i++){
		memset(recv_buf,0,1024);
		int num=recvfrom(com_fd,recv_buf,1024,0,(struct sockaddr*)&clt_addr,&addr_len);
		printf("Message from client (%d)) :%s\n",num,recv_buf);	
	}

	close(com_fd);
	
	unlink(UNIX_DOMAIN);
	return 0;
}
