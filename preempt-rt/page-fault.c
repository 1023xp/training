#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

typedef void (*func_ptr) (void);

void make_realtime(void)
{
	struct sched_param param = { 99 };
	if (sched_setscheduler(getpid(), SCHED_RR, &param) < 0) {
		perror("could not get real-time priority");
		exit(1);
	}
	if (mlockall(MCL_CURRENT | MCL_FUTURE) < 0) {
		perror("could not lock process in memory");
		exit(2);
	}
}

int getpagefaults()
{
	struct rusage theusage;
	if (getrusage(RUSAGE_SELF, &theusage) < 0) {
		perror("could not get rusage");
		exit(1);
	}

	return theusage.ru_majflt + theusage.ru_minflt;
}

void malloc_buffer()
{
	int bufsize = 4096 * 1000;
	int i;
	char *space;

	space = malloc(bufsize);
	for (i = 0; i < bufsize; i += 4096)
		space[i]++;
}

int getfaults(func_ptr f)
{
	int before;
	before = getpagefaults();
	f();
	return getpagefaults() - before;
}

void open_files(void)
{
	int i, fd;
	char filename[100];
	for (i = 0; i < 1000; i++) {
		sprintf(filename, "/tmp/foo%d", i);
		fd = open(filename, O_CREAT, 0400);
		close(fd);
		unlink(filename);
	}
}

void fopen_files(void)
{
	int i;
	FILE *fptr;
	int fd = open("/tmp/foo", O_CREAT, 0400);
	for (i = 0; i < 1000; i++) {
		fptr = fopen("/tmp/foo", "r");
		fclose(fptr);
	}
}

void read_files(void)
{
	int fd;
	struct dirent *direntry;
	DIR *dir;
#define READ_SIZE (1024*1000)
	char *buffer;
	struct stat statbuf;
	int count = 0;
	int bufsize;

	chdir("/tmp/big");
	dir = opendir("/tmp/big");
	if (dir == NULL) {
		perror("opening dir /tmp/big");
		exit(1);
	}
	while (direntry = readdir(dir)) {
		fd = open(direntry->d_name, O_RDONLY);
		fstat(fd, &statbuf);
		fflush(stdout);
		// have buffer no bigger than 100MB
		bufsize =
		    statbuf.st_size >
		    (1024 * 1024 * 100) ? (1024 * 1024 * 100) : statbuf.st_size;
		buffer = malloc(bufsize);
		if (buffer == NULL) {
			close(fd);
			continue;
		}
		read(fd, buffer, bufsize);
		free(buffer);
		close(fd);
	}
}

void math_funcs(void)
{
	double x;
	x = cos(1.1);
	x = sqrt(x);
	x = log(x);
}

void get_lf(void)
{
	char buf[2];
	printf("hit enter\n");
	fgets(buf, 2, stdin);
}

void mmap_files()
{
	int fd;
	struct dirent *direntry;
	DIR *dir;
	char x, *map_ptr;
#define READ_SIZE (1024*1000)
	char *buffer;
	struct stat statbuf;
	int pagecount = 0;
	const int numdirs = 1;
	char *dirs[numdirs];
	int j;

	dirs[0] = "/tmp/big";

	for (j = 0; j < numdirs; j++) {
		chdir(dirs[j]);
		dir = opendir(dirs[j]);
		if (dir == NULL) {
			perror("opening dir /tmp/big");
			exit(1);
		}
		while (direntry = readdir(dir)) {
			int winsize = 4096 * 1000;	// map 1000 pages at most
			int k;

			fd = open(direntry->d_name, O_RDONLY);
			//printf ("opened %d\n",fd);
			fstat(fd, &statbuf);
			for (k = 0; k < statbuf.st_size; k += winsize) {
				int mapsize =
				    (k + winsize) >
				    statbuf.st_size ? (statbuf.st_size -
						       k) : winsize;
				//printf("mapping at %d for %d bytes ",k,mapsize);
				map_ptr =
				    mmap(NULL, mapsize, PROT_READ, MAP_PRIVATE,
					 fd, k);
				//printf("got address = %x\n",map_ptr);

				if (((long)map_ptr) == -1) {
					//  perror("mmap"); // get errors on subdirs
				} else {
					int i;
					for (i = 0; i < mapsize; i += 4096) {
						//printf("i=%d, mapsize=%d filesize = %d file offset =%d offset+i=%d\n",
						//       i,mapsize,statbuf.st_size,k,k+i);
						x = map_ptr[i];
						pagecount++;
					}
					//printf("unmapping at %x for %d bytes\n",map_ptr, mapsize);
					if (munmap(map_ptr, mapsize) < 0) {
						perror("munmap");
						exit(1);
					}
				}
			}
			//printf ("closing %d\n",fd);
			close(fd);
		}
	}

	printf("touched %d pages = %dMB\n", pagecount,
	       pagecount / (1024 * 1024 / 4096));
}

int main()
{
	int fault_count;
	int i;

	printf("page faults for malloc %d\n", getfaults(malloc_buffer));
	get_lf();
	printf("page faults realtime = %d\n", getfaults(make_realtime));
	get_lf();
	printf("page faults open files = %d\n", getfaults(open_files));
	get_lf();
	printf("page faults fopen files = %d\n", getfaults(fopen_files));
	get_lf();
	printf("page faults math functions = %d\n", getfaults(math_funcs));
	get_lf();
	printf("page faults read files = %d\n", getfaults(read_files));
	get_lf();
	printf("page faults mmap files = %d\n", getfaults(mmap_files));

	return 0;
}
