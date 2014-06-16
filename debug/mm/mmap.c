#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

main()
{
	volatile unsigned char *m;
	/* 
	 * important notes:
	 * this needs sudo permission
	 */
	int fd = open("/dev/sda", O_RDONLY, S_IRUSR);
	unsigned int i;
#define MAP_SIZE 2048UL * 1024 * 1024
	m = mmap(0, MAP_SIZE, PROT_READ | PROT_EXEC,
			MAP_PRIVATE, fd, 0);

	sleep(1);
#if 1
	/* 
	 * enable this will trigger OOM killer
	 * disable this will trigger Page Reclaim
	 */
	mlockall(MCL_CURRENT | MCL_FUTURE);
#endif
	printf("memory base:%p\n", m);

	for (i = 0 ; i < MAP_SIZE; i++)
		m[i];

	while (1)
		sleep(1);
}
