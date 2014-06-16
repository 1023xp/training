#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main(void)
{
	key_t key;
	int proj_id;
	char* pathname="./p11.1.c";

	proj_id=1;

	key=ftok(pathname,proj_id);
	if(key==-1){
		perror("cannot generate the IPC key");
		return 1;
	}

	printf("proj_id= %d pathname=%s IPC key=%d\n",proj_id,pathname,key);

	proj_id=2;
	key=ftok(pathname,proj_id);
        if(key==-1){
                perror("cannot generate the IPC key");
                return 1;
        }

        printf("proj_id= %d pathname=%s IPC key=%d\n",proj_id,pathname,key);
}
