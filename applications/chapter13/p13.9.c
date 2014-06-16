#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
   int sock, length, n;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[256];
   
   if (argc != 3) { 
	   printf("Usage: %s server_ip  port_num\n",argv[0]);
           return 1;
   }

   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) {
   	   perror("cannot create communicating socket");
	   return 1;
   }

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) {
   	   perror("cannot get the server ip address");
	   return 1;
   }

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);

   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
   
   printf("(client) enter the message: ");
   bzero(buffer,256);
   fgets(buffer,255,stdin);

   n=sendto(sock,buffer,strlen(buffer),0,&server,length);
   if (n < 0){
   	perror("cannot get message from the client");
	return 1;
   }

   bzero(buffer,256);
   n = recvfrom(sock,buffer,256,0,&from, &length);
   if (n < 0) {
   	perror("cannot send message to the server");
	return 1;
   }

   printf("client got message : %s\n",buffer);
   close(sock);
   return 0;
}

