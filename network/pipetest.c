/*
 *  PipeTest by Davide Libenzi ( Epoll performace tester )
 *  Copyright (C) 1999,..,2003  Davide Libenzi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Davide Libenzi <davidel@xmailserver.org>
 *
 *
 *  You need either a never glibc or the epoll library available here :
 *
 *  http://www.xmailserver.org/linux-patches/nio-improve.html#sys_epoll
 *
 *  to build this source file. To build :
 *
 *  gcc -o pipetest pipetest.c -lepoll
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/epoll.h>


#define RUNTIMES 16


static int count, writes, fired;
static int *pipes;
static int num_pipes, num_active, num_writes;
static int epfd;
static struct epoll_event *events;



unsigned long long getustime(void) {
	struct timeval tm;

	gettimeofday(&tm, NULL);
	return (unsigned long long) tm.tv_sec * 1000000ULL + (unsigned long long) tm.tv_usec;
}


void read_cb(int fd, int idx) {
	int widx = idx + num_active + 1;
	u_char ch;

	if (read(fd, &ch, sizeof(ch)))
		count++;
	else
		fprintf(stderr, "false read event: fd=%d idx=%d\n", fd, idx);
	if (writes) {
		if (widx >= num_pipes)
			widx -= num_pipes;
		write(pipes[2 * widx + 1], "e", 1);
		writes--;
		fired++;
	}
}


int run_once(long *work, unsigned long long *tr) {
	int i, res;
	unsigned long long ts, te;

	fired = 0;
	for (i = 0; i < num_active; i++, fired++)
		write(pipes[i * 2 + 1], "e", 1);

	count = 0;
	writes = num_writes;

	ts = getustime();
	do {
		res = epoll_wait(epfd, events, num_pipes, 0);
		for (i = 0; i < res; i++)
			read_cb(pipes[2 * events[i].data.u32], events[i].data.u32);
	} while (count != fired);
	te = getustime();

	*tr = te - ts;
	*work = count;

	return (0);
}


int main (int argc, char **argv) {
	struct rlimit rl;
	int i, c;
	long work;
	unsigned long long tr;
	int *cp;
	struct epoll_event ev;
	extern char *optarg;

	num_pipes = 100;
	num_active = 1;
	num_writes = num_pipes;
	while ((c = getopt(argc, argv, "n:a:w:")) != -1) {
		switch (c) {
		case 'n':
			num_pipes = atoi(optarg);
			break;
		case 'a':
			num_active = atoi(optarg);
			break;
		case 'w':
			num_writes = atoi(optarg);
			break;
		default:
			fprintf(stderr, "Illegal argument \"%c\"\n", c);
			exit(1);
		}
	}

	rl.rlim_cur = rl.rlim_max = num_pipes * 2 + 50;
	if (setrlimit(RLIMIT_NOFILE, &rl) == -1) {
		perror("setrlimit"); 
		exit(1);
	}

	events = calloc(num_pipes, sizeof(struct epoll_event));
	pipes = calloc(num_pipes * 2, sizeof(int));
	if (events == NULL || pipes == NULL) {
		perror("malloc");
		exit(1);
	}

	if ((epfd = epoll_create(num_pipes)) == -1) {
		perror("epoll_create");
		exit(1);
	}

	for (cp = pipes, i = 0; i < num_pipes; i++, cp += 2) {
		if (pipe(cp) == -1) {
			perror("pipe");
			exit(1);
		}
		fcntl(cp[0], F_SETFL, fcntl(cp[0], F_GETFL) | O_NONBLOCK);
	}

	for (cp = pipes, i = 0; i < num_pipes; i++, cp += 2) {
		ev.events = EPOLLIN;
		ev.data.u32 = i;
		if (epoll_ctl(epfd, EPOLL_CTL_ADD, cp[0], &ev) < 0) {
			perror("epoll_ctl");
			exit(1);
		}
	}

	for (i = 0; i < RUNTIMES; i++) {
		run_once(&work, &tr);
		if (!work)
			exit(1);
		fprintf(stdout, "%lf\n", (double) tr / (double) work);
	}

	exit(0);
}

