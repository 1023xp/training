#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <stdlib.h>

/*
   A simple program to categorize the jitter in waking from a sleep
 */

#define get_tsc() \
    ({ \
        unsigned long low, high; \
        asm volatile("rdtsc" : "=d" (high), "=a" (low)); \
        ((unsigned long long)high << 32) | low; \
    })

#define N 1000
#define MSECS (1000*1000)
int khz;
int box[6] = { 0, 0, 0, 0, 0, 0 };

int classify(double diff)
{
	double adjust = diff;
	if (adjust < 0.0)
		adjust = -adjust;

	if (adjust > (100.0)) {
		return (5);
	}
	if (adjust > 10.0) {
		return (4);
	}
	if (adjust > 1.0) {
		return (3);
	}
	if (adjust > 0.1) {
		return (2);
	}
	if (adjust > 0.01) {
		return (1);
	} else {
		return (0);
	}
}

void calibrate()
{
	double t1, t2;
	struct timespec dummy = { 0, 0 };
	int i;

	t1 = get_tsc();
	for (i = 0; i < 1000; i++) {
		nanosleep(&dummy, NULL);
	}
	t2 = get_tsc();

	printf("t1 = %6.4f\nt2=%6.4f\n", t1, t2);
	printf("Loop 0f 1000 nanosleeps took %6.4fmsecs\n", (t2 - t1) / khz);
}

void check_jitter()
{
	int i;
	double t1, t2;
	double jitter[N];
	double max = 0;
	double sum = 0;
	double min = khz * 1000.0;
	struct timespec ten_ms;

	ten_ms.tv_sec = 0;
	ten_ms.tv_nsec = MSECS * 10;	// 10 milliseconds
	for (i = 0; i < N; i++) {
		t1 = get_tsc();
		nanosleep(&ten_ms, NULL);
		t2 = get_tsc();
		jitter[i] = t2 - t1;

		// ADJUST THE LINE BELOW IF NEEDED
		jitter[i] = (jitter[i] / khz) - 10;	//if ave not about 0.0 change the -10 
		//

		max = (jitter[i] > max) ? jitter[i] : max;
		min = (jitter[i] < min) ? jitter[i] : min;
		sum += jitter[i];
		box[classify(jitter[i])]++;
	}
	printf("max = %6.4f msecs, min = %6.4f msecs, ave = %6.4f msecs\n", max,
			min, (sum) / N);

	printf
		("Number >100 msecs  >10 msecs  >1 msec  >100 usec  >10 usec <=10 usec\n");
	printf
		("        %d           %d          %d         %d         %d        %d\n",
		 box[5], box[4], box[3], box[2], box[1], box[0]);

}

int main(int argc, char *argv[])
{
	struct sched_param sched_params;
	int retval;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s cpu_speed_in_Khz [priority]\n",
				argv[0]);
		exit(-1);
	}

	khz = atoi(argv[1]);
	if (argc > 2) {
		sched_params.sched_priority = atoi(argv[2]);
		if ((retval =
					sched_setscheduler(getpid(), SCHED_RR,
						&sched_params)) < 0) {
			perror("sched_setscheduler");
			return (-1);
		}
		printf("Using priority = %d\n", sched_params.sched_priority);
	} else
		printf("Using Timeshare policy\n");

	if ((retval = mlockall(MCL_CURRENT | MCL_FUTURE)) < 0) {
		perror("mlockall");
		return (-1);
	}

	printf("Max priority = %d\n", sched_get_priority_max(SCHED_RR));
	//calibrate();
	check_jitter();

}
