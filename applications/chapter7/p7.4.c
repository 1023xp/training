#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
	printf("Current process UID : %ld\n",(long)getuid());
	printf("Current porcess EUID : %ld\n",(long)geteuid());

	return 0;
}
