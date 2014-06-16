#include "pagesize.h"

extern int do_shared_array_work(void);

/* We'd like the arrays big enough that the page granularity doesn't
 * skew the figures */
#define ARRAY_SIZE (CURRENT_PAGE_SIZE * 100)

/* And to make it really easy, ask gcc to put things on page boundaries */
typedef unsigned char BigArr[ARRAY_SIZE] __attribute__ ((aligned(CURRENT_PAGE_SIZE)));

