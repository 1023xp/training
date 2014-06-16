/* (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license */
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

int main(int argc, char *argv[])
{
	int total_size = 100 * 1024;
	struct timeval pause_time;
	int i;
	char *p;
	int loops_per_sec = 10;
	int duration_secs = 60;
	int loops = duration_secs * loops_per_sec;
	
	if (argc > 1) {
		total_size = atoi(argv[1]);
	}

	p = malloc(total_size);
	while (loops-- > 0) {
		for (i = 0; i < total_size; ++i) {
			p[i] = i % 128;
		}
		pause_time.tv_sec = 0;
		pause_time.tv_usec = 1000000 / loops_per_sec;
		select(0, NULL, NULL, NULL, &pause_time);
	}
	return 0;
}
