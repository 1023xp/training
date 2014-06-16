#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <aio.h>

#define DATA_LEN 1024
#define DATA_SIZE 1024

char bufferAO[8192] __attribute__((aligned(4096)));

void AIO_write()
{
	struct timeval start;
	struct timeval end;
	int ret;
	int i = 0;

	struct aioinit;

	int dwFlags = O_RDWR |  O_LARGEFILE | O_CREAT;

	int fd = open("aio_write.dat", dwFlags);
	if (fd<0)
	{
		perror("open error:");
		return;
	}

	if (ftruncate64(fd, DATA_LEN * DATA_SIZE))
	{
		perror("ftruncate64 error:");
		return;
	}
	
	gettimeofday(&start, NULL);

	for (; i<= DATA_SIZE; i++)
	{
		struct aiocb stAio;
		memset(&stAio, 0, sizeof(stAio));
		stAio.aio_buf = bufferAO;
		stAio.aio_nbytes = DATA_LEN;
		stAio.aio_offset = i * DATA_LEN;
		stAio.aio_fildes = fd;

		ret = aio_write(&stAio);

		if(ret < 0)
		{
			perror("aio_write error:");
			return;
		}

		struct aiocb *cblist[1] = {&stAio};
		/* 查看一个异步请求的状态（进行中EINPROGRESS？还是已经结束或出错？）*/
		ret = aio_error(&stAio);

		if (ret == 0) {
			// already completed
		}
		else if (ret == EINPROGRESS) {
			/* 阻塞等待请求完成 */
			if (aio_suspend((const struct aiocb *const*)cblist, 1, NULL))
			{
				perror("aio_suspend error:");
				return;
			}
		}
		else {
			perror("aio_error error:");
			return;
		}
		/* 查看一个异步请求的返回值（跟同步读写定义的一样） */
		if (aio_return(&stAio) != DATA_LEN)
		{
			perror("aio_return error:");
			return;
		}
	}
}

void AIO_read(void)
{
	int fd, ret;
	struct aiocb my_aiocb;

#define BUFF_SIZE (1024 * 1024 * 20)
	fd = open("aio_write.dat", O_RDONLY);
	if (fd < 0) perror("open");

	bzero((char *)&my_aiocb, sizeof(struct aiocb));
	my_aiocb.aio_buf = malloc(BUFF_SIZE + 1);
	if (!my_aiocb.aio_buf) perror("malloc");

	my_aiocb.aio_fildes = fd;
	my_aiocb.aio_nbytes = BUFF_SIZE;
	my_aiocb.aio_offset = 0;

	ret = aio_read(&my_aiocb);
	if (ret < 0) perror("aio_read");

	/*
	 * 查看一个异步请求的状态（进行中EINPROGRESS？还是已经结束或出错？
	 */
	while (aio_error(&my_aiocb) == EINPROGRESS);

	if ((ret = aio_return(&my_aiocb)) <= 0)
		printf("read failed\n");
}

main()
{
	AIO_write();
	AIO_read();
}
