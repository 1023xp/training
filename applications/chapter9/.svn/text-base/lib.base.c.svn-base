#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Daemonize myself. */
int
daemon (int nochdir, int noclose)
{
  pid_t pid;

  pid = fork ();

  /* In case of fork is error. */
  if (pid < 0)
    {
      perror ("fork");
      return -1;
    }

  /* In case of this is parent process. */
  if (pid != 0)
    exit (0);

  /* Become session leader and get pid. */
  pid = setsid();

  if (pid < -1)
    {
      perror ("setsid");
      return -1;
    }

  /* Change directory to root. */
  if (! nochdir)
    chdir ("/");

  /* File descriptor close. */
  if (! noclose)
    {
      int fd;

      fd = open("/dev/null",O_RDWR,0);
      if (fd != -1)
	{
	  dup2 (fd, STDIN_FILENO);
	  dup2 (fd, STDOUT_FILENO);
	  dup2 (fd, STDERR_FILENO);
	  if (fd > 2)
	    close (fd);
	}
    }

  umask (0027);

  return 0;
}

/* Signale wrapper. */
void * signal_set (int signo, void (*func)(int))
{
  int ret;
  struct sigaction sig;
  struct sigaction osig;

  sig.sa_handler = func;
  sigemptyset (&sig.sa_mask);
  sig.sa_flags = 0;
#ifdef SA_RESTART
  sig.sa_flags |= SA_RESTART;
#endif /* SA_RESTART */

  ret = sigaction (signo, &sig, &osig);

  if (ret < 0) 
    return (SIG_ERR);
  else
    return (osig.sa_handler);
}

void sigint(int sig)
{
	//do something,for example send message to syslogd
	
}

void sigtstp(int sig)
{
	//do someting, for example send message to syslogd
}

/* Initialization of signal handles. */
void
signal_init ()
{
       	signal_set (SIGINT, sigint);
	signal_set (SIGTSTP, sigtstp);
}


