#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

int main(int argc, char *argv[])
{
	int a = 1, b = 2, c = 3;

	if(likely(a)) {
		b++;
		b++;
		b++;
		b++;
	} else {
		c--;
		c--;
		c--;
		c--;
	}

	printf("%d %d\n", b, c);
}
