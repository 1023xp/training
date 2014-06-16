#include <stdio.h>
#include <unistd.h>

int main()
{
    char *comments[] = {
	"This file is auto-generated. Please do not edit.",
	"This value is the result of the getpagesize() call.",
	"We need this trickery to get a compile-time value for the page size.",
	NULL
    };
    char **p = comments;
    while (*p != NULL) {
	printf("/* %s */\n", *p);
	p++;
    }
    printf("\n");

    printf("#define CURRENT_PAGE_SIZE (%d)\n", getpagesize());
    return 0;
}

