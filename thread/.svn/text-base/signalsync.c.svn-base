#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

void sig_handler(int signum)
{
	static int j = 0;
	static int k = 0;
	pthread_t  sig_ppid = pthread_self();
	// used to show which thread the signal is handled in.

	if (signum == SIGUSR1) {
		printf("thread %d, receive SIGUSR1 No. %d\n", sig_ppid, j);
		j++;
		//SIGRTMIN should not be considered constants from userland,
		//there is compile error when use switch case
	} else if (signum == SIGRTMIN) {
		printf("thread %d, receive SIGRTMIN No. %d\n", sig_ppid, k);
		k++;
	}
}

void* worker_thread()
{
	pthread_t  ppid = pthread_self();
	pthread_detach(ppid);
	while (1) {
		printf("I'm thread %d, I'm alive\n", ppid);
		sleep(10);
	}
}

void* sigmgr_thread()
{
	sigset_t   waitset, oset;
	siginfo_t  info;
	int        rc;
	pthread_t  ppid = pthread_self();

	pthread_detach(ppid);

	sigemptyset(&waitset);
	sigaddset(&waitset, SIGRTMIN);
	sigaddset(&waitset, SIGUSR1);

	while (1)  {
		rc = sigwaitinfo(&waitset, &info);
		if (rc != -1) {
			printf("sigwaitinfo() fetch the signal - %d\n", rc);
			sig_handler(info.si_signo);
		} else {
			printf("sigwaitinfo() returned err: %d; %s\n", errno, strerror(errno));
		}
	}
}


int main()
{
	sigset_t bset, oset;
	int             i;
	pid_t           pid = getpid();
	pthread_t       ppid;


	// Block SIGRTMIN and SIGUSR1 which will be handled in
	//dedicated thread sigmgr_thread()
	// Newly created threads will inherit the pthread mask from its creator
	sigemptyset(&bset);
	sigaddset(&bset, SIGRTMIN);
	sigaddset(&bset, SIGUSR1);
	//A new thread inherits a copy of its creator's signal mask.
	if (pthread_sigmask(SIG_BLOCK, &bset, &oset) != 0)
		printf("!! Set pthread mask failed\n");

	// Create the dedicated thread sigmgr_thread() which will handle
	// SIGUSR1 and SIGRTMIN synchronously
	pthread_create(&ppid, NULL, sigmgr_thread, NULL);

	// Create 5 worker threads, which will inherit the thread mask of
	// the creator main thread
	for (i = 0; i < 5; i++) {
		pthread_create(&ppid, NULL, worker_thread, NULL);
	}

	// send out 50 SIGUSR1 and SIGRTMIN signals
	for (i = 0; i < 50; i++) {
		kill(pid, SIGUSR1);
		printf("main thread, send SIGUSR1 No. %d\n", i);
		kill(pid, SIGRTMIN);
		printf("main thread, send SIGRTMIN No. %d\n", i);
		sleep(10);
	}
	exit (0);
}

