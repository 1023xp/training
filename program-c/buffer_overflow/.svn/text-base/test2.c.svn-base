#include <string.h>

func()
{
	int x[16];
	x[18] = 0xFF;
	
	printf("%p %p\n", x, &x[18]);
}

int main(int argc, char *argv[])
{
	int x[16], y[16], z[16];

	printf("%p %p %p\n", x, y, z);

	func();
	return 0;
}
