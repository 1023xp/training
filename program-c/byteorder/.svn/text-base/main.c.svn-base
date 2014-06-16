union {

	struct BYTE {
		unsigned char h;
		unsigned char l;
	} S;
	unsigned short s;
} test;

main()
{
	test.S.h = 0xFF;
	test.S.l = 0x0;
	if (test.s == 0xFF)
		printf("little byteorder\n");
	else
		printf("big byteorder\n");
}
