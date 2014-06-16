#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void do_at_exit(void) 
{
	printf("You can see the output when the program terminates\n");
}


int main(){
	int flag;

	flag=atexit(do_at_exit);

	if (flag != 0) {
		printf("Cannot set exit function\n");
	 	return EXIT_FAILURE;
	}
         
  	_exit(EXIT_SUCCESS);
}
