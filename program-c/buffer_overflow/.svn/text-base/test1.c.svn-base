#include <string.h>

int main(int argc, char *argv[])
{
	int x[16], y[16], z[16];

	printf("%p %p %p %p\n", x, y, z, &y[16]);
	/* array overflow */
	y[16] = 0;

	return 0;
}
