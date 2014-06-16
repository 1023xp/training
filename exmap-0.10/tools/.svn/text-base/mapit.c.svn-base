/* (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license */
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <alloca.h>
#include <string.h>
#include <unistd.h>

#define FILE_SIZE (4096 * 200)

static int create_file(char *fname, int size);

int main(int argc, char **argv)
{
	char *p;
	char *fname = "./mapit.dat";
	int i;
	int sum;
	
	int fd = create_file(fname, FILE_SIZE);

	if (fd < 0) return -1;

	p = mmap(NULL, FILE_SIZE, PROT_READ, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED || p == NULL) {
		perror("mmap");
		return -1;
	}
	/* Ensure it's mapped in */
	sum = 0;
	for (i = 0; i < FILE_SIZE; ++i) {
		sum += p[i];
	}

	getchar();
	munmap(p, FILE_SIZE);
	unlink(fname);
	
	return 0;
}


static int create_file(char *fname, int size)
{
	int i;
	int fd;
	int wsize;
	void *buffer = alloca(BUFSIZ);

	if (buffer == NULL) return -1;
	memset(buffer, 'a', BUFSIZ);
	
	fd = open(fname, O_RDWR | O_CREAT, 0644);
	if (fd < 0) {
		perror("open");
		return -1;
	}

	wsize = BUFSIZ;
	for (i = size; i > 0; i -= BUFSIZ) {
		if (i < BUFSIZ) wsize = i;
		if (write(fd, buffer, wsize) != wsize) {
			perror("write");
			return -1;
		}
	}

	return fd;
}
		
