#include <string.h>

func()
{
	int x[16];
	x[48] = 0xFF;
	
	printf("%p %p\n", x, &x[48]);
}

int main(int argc, char *argv[])
{
	int x[16], y[16], z[16];

	printf("%p %p %p\n", x, y, z);

	func();
	return 0;
}
