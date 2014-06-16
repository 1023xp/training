#include <stdio.h>

int main()
{
    fprintf(stdout, "Hello world! fprintf=%p, stdout=%p\n", fprintf, stdout);
    return 0;
}
