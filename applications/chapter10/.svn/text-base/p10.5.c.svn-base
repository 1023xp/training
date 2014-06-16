#include <stdio.h>

#define BUFFER_SIZE 300

int main(int argc,char* argv[])
{
	FILE* stream_in;
	FILE* stream_out;
	int nLen;

	char buffer[BUFFER_SIZE];

	if(argc!=3){
		printf("Usage: %s command1 command2\n",argv[0]);
		return 1;
	}	

	stream_in=popen(argv[1],"r");
	if(stream_in==NULL){
		perror("call popen function fail");
		return 1;
	}

	stream_out=popen(argv[2],"w");
	if(stream_out==NULL){
		perror("call popen function fail");
		return 1;
	}

	fflush(stream_out);

	nLen=read(fileno(stream_in),buffer,PIPE_BUF);
	while(nLen>0){
		write(fileno(stream_out),buffer,nLen);
		nLen=read(fileno(stream_in),buffer,PIPE_BUF);	
	}

	pclose(stream_in);
	pclose(stream_out);

	return 0;
}
