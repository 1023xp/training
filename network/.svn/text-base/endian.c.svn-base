#include <stdio.h>

int is_little_endian(void)
{
	unsigned short flag=0x4321;

	if(*(unsigned char*)&flag==0x21)
		return 1;
	else
		return 0;
}

int main(void)
{
	union endian_un{
		short var;
		char bits[sizeof(short)];
	};

	union endian_un flag;

	flag.var=0x0102;

	if(sizeof(short)==2){
		if(flag.bits[0]==1 && flag.bits[1]==2)
			printf("judged by first method, big-endian\n");
		else if(flag.bits[0]==2 && flag.bits[1]==1)
			printf("judged by first method, little-endian\n");
		else
			printf("cannot determine the type\n");
	}

	if(is_little_endian())
		printf("judged by second method, little-endian\n");
	else
		printf("judged by second method, big-endian\n");

	return 0;
}
