#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

int main(void){
	struct termios term;
	int err;

	if(tcgetattr(STDIN_FILENO,&term)==-1){
		perror("Cannot get standard input description");
		return 1;
	}

	term.c_cc[VEOF]=(cc_t)0x07;

	err=tcsetattr(STDIN_FILENO,TCSAFLUSH,&term);

	if(err==-1 && err==EINTR){
		perror("Failed to change EOF character");
		return 1;
	}

	return 0;
}


