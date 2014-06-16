#include <stdio.h>
#include <string.h>

struct test1
{
	char x1;
	short x2;
	float x3;
	char x4;
};

#pragma pack(1)
struct test2
{
	char x1;
	short x2;
	float x3;
	char x4;
};
#pragma pack()

#pragma pack(8)
struct test3
{
	char x1;
	short x2;
	float x3;
	char x4;
};
#pragma pack()

#pragma pack(1)
struct test4
{
	char x1;
	short x2;
	float x3;
	long long x4;
};
#pragma pack()

#pragma pack(1) // 按照1字节方式进行对齐
struct TCPHEADER
{
	short SrcPort; // 16位源端口号
	short DstPort; // 16位目的端口号
	int SerialNo; // 32位序列号
	int AckNo; // 32位确认号
	unsigned char HaderLen : 4; // 4位首部长度
	unsigned char Reserved1 : 4; // 保留6位中的4位 [Page]
	unsigned char Reserved2 : 2; // 保留6位中的2位
	unsigned char URG : 1;
	unsigned char ACK : 1;
	unsigned char PSH : 1;
	unsigned char RST : 1;
	unsigned char SYN : 1;
	unsigned char FIN : 1;
	short WindowSize; // 16位窗口大小
	short TcpChkSum; // 16位TCP检验和
	short UrgentPointer; // 16位紧急指针
};
#pragma pack() // 取消1字节对齐方式 

struct stu {
	char sex;
	int length;
	char name[10];
} __attribute__((packed)); 

struct stu1 {
	char sex;
	int length;
	char name[10];
} __attribute__((aligned (8))); 

struct stu2 {
	char sex;
	long long int length;
	char name[10];
}; 

struct stu3 {
	char sex;
	int tmp;
	long long int length;
	char name[10];
}; 

int main(int argc, char *argv[])
{
	{
		struct test1 *p = 0;
		printf("struct test1 length:%d, offset: %p, %p, %p, %p\n", sizeof(*p),
				(void *)&p->x1, (void *)&p->x2, (void *)&p->x3, (void *)&p->x4);
	}

	{
		struct test2 *p = 0;
		printf("struct test2 length:%d, offset: %p, %p, %p, %p\n", sizeof(*p),
				(void *)&p->x1, (void *)&p->x2, (void *)&p->x3, (void *)&p->x4);
	}

	{
		struct test3 *p = 0;
		printf("struct test3 length:%d, offset: %p, %p, %p, %p\n", sizeof(*p),
				(void *)&p->x1, (void *)&p->x2, (void *)&p->x3, (void *)&p->x4);
	}
	{
		struct test4 *p = 0;
		printf("struct test4 length:%d, offset: %p, %p, %p, %p\n", sizeof(*p),
				(void *)&p->x1, (void *)&p->x2, (void *)&p->x3, (void *)&p->x4);
	}
	{
	struct stu my_stu;
	printf("%d\n", sizeof(my_stu));
	}
	{
	struct stu1 my_stu1;
	printf("%d %p len:%p\n", sizeof(my_stu1), &my_stu1, &my_stu1.length);
	}
	{
	struct stu2 my_stu2;
	printf("%d %p len:%p\n", sizeof(my_stu2), &my_stu2, &my_stu2.length);
	}
	{
	struct stu3 my_stu3;
	printf("%d %p len:%p\n", sizeof(my_stu3), &my_stu3, &my_stu3.length);
	}
}
