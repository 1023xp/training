int test_no_restrict(int* x, int* y)
{
	*x = 0;
	*y = 1;
	return *x;
}

int test_have_restrict(int* restrict x, int* restrict y)
{
	*x = 0;
	*y = 1;
	return *x;
}

int main(int argc, char *argv[])
{
	int a, b;
	test_no_restrict(&a, &b);
	test_have_restrict(&a, &b);
}
