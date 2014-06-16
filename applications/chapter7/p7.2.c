#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int etext,edata,end;

char *g_pstr="Global string\n";
char g_buffer[20];

inline void disp_addr(char* p,int addr)
{
	printf("Name :%s, Address :%0xd\n",p,addr);
}

void disp_var(char* p){
	char *buffer1;
	
	disp_addr("buffer1 address:",(int)(&buffer1));
	buffer1=(char*)malloc(strlen(p)+1);
	strcpy(buffer1,p);
	printf("buffer1 : %s\n",buffer1);
	free(buffer1);
}

int main(void){
	int i=0;
	
	printf("Addr etext :%0xd\n",(int)(&etext));
	printf("Addr edata :%0xd\n",(int)(&edata));
	printf("Addr end   :%0xd\n",(int)(&end));
	
	disp_addr("function main() :",(int)(main));
	disp_addr("function disp_var():",(int)(disp_var));
	disp_addr("g_pstr address: ",(int)(&g_pstr));
	disp_addr("g_buffer address: ",(int)(&g_buffer));
	disp_addr("i address: ",(int)(&i));

	disp_var(g_pstr);
	return 0;	

}
