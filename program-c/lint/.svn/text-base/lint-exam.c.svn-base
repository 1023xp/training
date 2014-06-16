#include<stdlib.h>
#include<stdio.h>

/*Program: typeerr.c -- use splint to check type varible error */
int foo(int i, char *s, bool b1, bool b2)
{
	if (i = 3)
		return b1;
	if (!i || s)
		return i;
	if (s)
		return 7;
	if (b1 == b2)
		return 3;
	return 2;
}

/*Program: overflow -- splint check overflow error */
int overflow()
{
	int buf[10];
	buf[10] = 3;
	return 0;
}

/*Program: bound.c -- use splint checking bound overflow error */
void updateEnv(char *str)
{
	char *tmp;
	tmp = getenv("MYENV");
	if (tmp != NULL)
		strcpy(str, tmp);
}

void updateEnvSafe(char *str, size_t strSize)
{
	char *tmp;
	tmp = getenv("MYENV");
	if (tmp != NULL) {
		strncpy(str, tmp, strSize - 1);
		str[strSize - 1] = '\0';
	}
}

/*Program: malloc1.c -- check varible type */
int main()
{
	char *some_mem;
	int size1 = 1048567;
	//size_t size1 = 1048567;
	some_mem = (char *)malloc(size1);
	printf("Malloced 1M Memory!\n");
	free(some_mem);
	exit(EXIT_SUCCESS);
}
