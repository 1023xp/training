#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int, siginfo_t *, void *);

int main(int argc, char **argv)
{
	struct sigaction act;
	sigset_t newmask, oldmask;
	int rc;

	sigemptyset(&newmask);

	//往信号集中添加一个非实时信号
	sigaddset(&newmask, SIGINT);

	//往信号集中添加一个实时信号
	sigaddset(&newmask, SIGRTMIN);

	//屏蔽实时信号SIGRTMIN
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);

	act.sa_sigaction = sig_handler;
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SIGINT, &act, NULL) < 0) {
		printf("install sigal error\n");
	}

	if (sigaction(SIGRTMIN, &act, NULL) < 0) {
		printf("install sigal error\n");
	}

	printf("pid = %d\n", getpid());

	//进程睡眠，在此时间内的发给该进程的所有实时信号
	//将排队，不会有信号丢失
	sleep(20);

	//解除对SIGRTMIN信号的屏蔽
	//信号处理函数将会被调用
	sigprocmask(SIG_SETMASK, &oldmask, NULL);

	return 0;
}

void sig_handler(int signum, siginfo_t * info, void *myact)
{
	if (signum == SIGINT)
		printf("Got a common signal\n");
	else
		printf("Got a real time signal\n");
}
