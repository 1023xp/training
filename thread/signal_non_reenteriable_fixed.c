#include <signal.h>
#include <stdio.h>
struct two_int { int a, b; } data;
void signal_handler(int signum){
	printf ("%d, %d\n", data.a, data.b);
	alarm (1);
}
int main (void){
	sigset_t   bset, oset;
	static struct two_int zeros = { 0, 0 }, ones = { 1, 1 };
	signal (SIGALRM, signal_handler);
	data = zeros;
	alarm (1);
	sigemptyset(&bset);
	sigaddset(&bset, SIGALRM);

	while (1) {
		if (pthread_sigmask(SIG_BLOCK, &bset, &oset) != 0)
			printf("!! Set pthread mask failed\n");
		data = zeros;
		if (pthread_sigmask(SIG_SETMASK, &oset, NULL) != 0)
			printf("!! Set pthread mask failed\n");

		if (pthread_sigmask(SIG_BLOCK, &bset, &oset) != 0)
			printf("!! Set pthread mask failed\n");
		data = ones;
		if (pthread_sigmask(SIG_SETMASK, &oset, NULL) != 0)
			printf("!! Set pthread mask failed\n");
	}
}

