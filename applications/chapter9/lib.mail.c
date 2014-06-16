#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <syslog.h>

#define SLEEP_TIME 5

//get the mail size
long get_file_size(const char* filename) 
{
	struct stat status; 
	long size;

	if(stat(filename,&status)==-1){
	       perror("cannot get the file status");
	       return -1;
	} 
	
	size=status.st_size;
	return size; 
} 

void mail_log(const char* progname,const char* mail_pos)
{
	fprintf(stderr,"%s notice: You hava new mail in %s\n",progname,mail_pos);
}


int check_mail(const char *filename,const char* progname) 
{ 
	long old_mail_size;
	long mail_size; 

	old_mail_size=get_file_size(filename); 

	if(old_mail_size==-1)
		return 1; 

	if(old_mail_size>0) 
		mail_log(progname,filename); 

	sleep(SLEEP_TIME); 

	while(1){ 
		mail_size=get_file_size(filename); 
		
		if(mail_size!=old_mail_size){
			mail_log(progname,filename); 
		}

		old_mail_size=mail_size; 
		
		sleep(SLEEP_TIME); 
	} 

} 

