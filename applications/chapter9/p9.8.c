#include <stdio.h>

#define USER_ACCOUNT "/var/mail/program"

extern void signal_init();
extern int daemon (int nochdir, int noclose);
int check_mail(const char *filename,const char* progname);

int main(int argc,char* argv[])
{
	char *p;
	int opt;
	int daemon_mode=1;
	char* progname;

	signal_init();

	if(daemon_mode)
		daemon(0,1);
	
	check_mail(USER_ACCOUNT,argv[0]);
}
