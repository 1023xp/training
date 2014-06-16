#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/kd.h>

#define SPEAKER_DEVICE "/dev/console"

int main(int argc,char* argv[]){
	//
	int fd;
	int i;	
	int freq;
	
	if(argc!=2){
		printf("Usage: %s frequence \n",argv[0]);
		return 1;
	}

	freq=atoi(argv[1]);

	if(freq<=0 || freq>10000){
		printf("the frequence must be in the range from 0 to 10000\n");
		return 1;
	}

	//
	fd=open(SPEAKER_DEVICE,O_WRONLY);

	if(fd==-1){
		perror("Cannot open the speaker device");
		return 1;
	}

	for(i=0;i<10;i++){
		//
		int set_freq=1190000/freq;		
 		//
		ioctl(fd,KIOCSOUND,set_freq);		
		//
		usleep(200);
		//
		ioctl(fd,KIOCSOUND,0);
		
		usleep(100);
	}
	
	return 0;
}
