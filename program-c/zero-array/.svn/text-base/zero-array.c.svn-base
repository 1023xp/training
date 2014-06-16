#include <stdio.h>
#include <string.h>
struct A
{
	int   a, b;
	char data[0];
};

int main(int argc, char *argv[])
{
	struct A *p;

	int n = 100, i;
	p = malloc(sizeof(struct A) + n);
	for (i = 0; i < n; ++i)
		p->data[i] = 1;
}
