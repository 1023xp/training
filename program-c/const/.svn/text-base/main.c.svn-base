#include <stdio.h>

const int g;

extern void add_g(void);

main()
{
/*	g = 1;*/

	printf("%s %d\n", __func__, g);
	add_g();
	printf("%s %d\n", __func__, g);
}
