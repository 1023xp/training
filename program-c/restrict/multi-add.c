#include <stdio.h>

void multi_add_r(int* restrict p1, int* restrict p2, int* restrict pi)
{
    *p1 += *pi;
    *p2 += *pi;
}

void multi_add_n(int* p1, int* p2, int* pi)
{
    *p1 += *pi;
    *p2 += *pi;
}

int main()
{
    int a = 1, b = 2;
    int inc = 1;

    // increase both a and b by 1
    multi_add_r(&a, &b, &inc);
    multi_add_n(&a, &b, &inc);

    // print the result
    printf("a = %d, b = %d\n", a, b);
}
