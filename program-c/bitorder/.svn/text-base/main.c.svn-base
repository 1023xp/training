union {

	struct BYTE {
		unsigned char h :4;
		unsigned char l :4;
	} B;
	unsigned char b;
} test;

main()
{
	test.B.h = 0xF;
	test.B.l = 0x0;
	if (test.b == 0x0F)
		printf("little bitorder\n");
	else
		printf("big bitorder\n");
}
