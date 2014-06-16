#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
   int sock, length, fromlen, n;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[1024];

   if (argc!= 2) {
      printf( "Usage: %s port_num\n",argv[0]);
      return 1;
   }
   
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0){ 
	perror("cannot create communicating socket");
	return 1;
   }

   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));

   if (bind(sock,(struct sockaddr *)&server,length)<0){ 
       perror("cannot bind the socket");
       close(sock);
       return 1;
   }

   fromlen = sizeof(struct sockaddr_in);
   
   while (1) {
       n = recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) {
       	      perror("cannot receive date from client");
	      break;
       }
       write(STDOUT_FILENO,"server: Received a datagram: ",29);
       write(STDOUT_FILENO,buf,n);
       
       n = sendto(sock,"send message to client\n",22,
                  0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) {
	      perror("cannot send data to the client");
	      break;
	}
   }

   close(sock);
   return 0;
 }

