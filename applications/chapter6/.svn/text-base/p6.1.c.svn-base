#include <stdio.h>
#include <unistd.h>

int main(int argc,char* argv[]){
	int fd;

	char* tty_out_name;
	char* tty_in_name;
	char* tty_err_name;

	if((tty_out_name=ttyname(STDOUT_FILENO))==NULL){
		perror("Cannot get tty name");
		return 1;
	}

	printf("STDOUT_FILENO tty name is :%s\n",tty_out_name);

	if((tty_in_name=ttyname(STDIN_FILENO))==NULL){
		perror("Cannot get tty name");
		return 1;
	}
	
	printf("STDIN_FILENO tty name is :%s\n",tty_in_name);

	if((tty_err_name=ttyname(STDERR_FILENO))==NULL){
		perror("Cannot get tty name");
		return 1;
	}

	printf("STDERR_FILENO tty name is :%s\n",tty_err_name);
	return 0;
}
