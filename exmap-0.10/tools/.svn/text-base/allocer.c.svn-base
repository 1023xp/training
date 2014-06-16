/* (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license */
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
     const int malloc_size = 100 * 4096;
     const int chunk_size = 2048; /* large allocs don't come from sbrk() */
     int total_size;

     for (total_size = 0; total_size < malloc_size; total_size += chunk_size) {
	  malloc(chunk_size);
     }
     sleep(60);
     return 0;
}
