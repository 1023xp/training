#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	int connect_fd;
	int ret;
	char snd_buf[1024];
	int i;
	int port;
	int len;

	static struct sockaddr_in srv_addr;

	if(argc!=3){
		printf("Usage: %s server_ip_address port\n",argv[0]);
		return 1;
	}	

	port=atoi(argv[2]);

	connect_fd=socket(PF_INET,SOCK_STREAM,0);
	if(connect_fd<0){
		perror("cannot create communication socket");
		return 1;
	}	

	memset(&srv_addr,0,sizeof(srv_addr));
	srv_addr.sin_family=AF_INET;
	srv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	srv_addr.sin_port=htons(port);

	ret=connect(connect_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
	if(ret==-1){
		perror("cannot connect to the server");
		close(connect_fd);
		return 1;
	}
	
	memset(snd_buf,0,1024);

	while(1){
		write(STDOUT_FILENO,"input message:",14);
		
		len=read(STDIN_FILENO,snd_buf,1024);
		if(len>0)
			write(connect_fd,snd_buf,len);

		len=read(connect_fd,snd_buf,len);
		if(len>0)
			printf("Message from server: %s\n",snd_buf);
		
		if(snd_buf[0]=='@')
			break;	
	}
	
	close(connect_fd);
	return 0;
}
