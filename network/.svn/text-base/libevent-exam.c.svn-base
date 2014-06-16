#include <sys/types.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <event.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define WEBMSG "HTTP/1.1 200 OK\r\nContent-Length: 29\r\nConnection: close\r\nContent-Type: text/html\r\n\r\nHello This is libevent server"

static void sock_response(int fd, short event, void *arg)
{
	char buffer[1500];

	struct event *ev = arg;
	read(fd, buffer, 1500);
	printf("%s\n", buffer);
	write(fd, WEBMSG, strlen(WEBMSG));
	event_del(ev);
	close(fd);
}

static void sock_connect(int fd, short event, void *arg)
{
	struct event *evfifo;
	struct sockaddr_in client;
	int connfd, clnlen;

	clnlen = sizeof(client);
	connfd = accept(fd, (struct sockaddr *)&client, &clnlen);
	if (connfd < 0) {
		perror("accept");
		return;
	}
	evfifo = malloc(sizeof(struct event));
	event_set(evfifo, connfd, EV_READ, sock_response, evfifo);
	event_add(evfifo, NULL);

}

int main(int argc, char *argv[])
{
	struct event evfifo;
	struct sockaddr_in serv;
	int sockfd;

/*initial server socket*/
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr("192.168.15.25");
	serv.sin_port = htons(5050);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("sockfd");
		return 0;
	}
	if (bind(sockfd, (struct sockaddr *)&serv, sizeof(serv)) == -1) {
		perror("bind");
		return 0;
	}
	if (listen(sockfd, 1024) == -1) {
		perror("listen");
		return 0;
	}
/* Initalize the event library */
	event_init();
	event_set(&evfifo, sockfd, EV_READ | EV_PERSIST, sock_connect, &evfifo);
/* Add it to the active events, without a timeout */
	event_add(&evfifo, NULL);

	event_dispatch();
	close(sockfd);
}
