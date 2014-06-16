#include <string.h>

int foo(int x)
{
	if (x < 0)
		printf("%d ", x);

	return 0;
}

int main(int argc, char *argv[])
{
	char *ptr, *ptr1;
	int x, y[10];

	/* malloc without free */
	ptr = (char*) malloc(10);
	strcpy(ptr, "01234567890");

	/* not initialized memory */
	foo(x);
	
	/* more free */
	ptr1 = (char*) malloc(10);
	ptr1[0] = '\0';
	free(ptr1);
	free(ptr1);
	
	/* array overflow */
	y[11] = 0;

	return 0;
}
