/*
 * rt_check.c -- Check for real-time capabilities on the running system.
 * $Id: $
 *
 * Returns 0 on success, and non-zero on failure.  The failure will indicate
 * what necessary feature was not present.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright © International Business Machines  Corp., 2006-2008
 *
 * Author(s):
 *	Theodore Ts'o
 *	Timothy R. Chavez <tinytim@us.ibm.com>
 *	Vernon Mauery <vernux@us.ibm.com>
 *	Darren Hart <dvhltc@us.ibm.com>
 *
 * Changelog:
 *	09/12/2008 - Introduced printing macros to reduce code size
 *	09/09/2008 - Added priority option and set SCHED_FIFO priority and
 *	             mlockall prior to executing tests.
 *	05/30/2007 - Don't cache user-dependent test results
 *	05/29/2007 - Revised for RHEL5-RT inclusion (v0.6)
 *	05/03/2007 - Revised for RHEL5-RT inclusion (v0.5)
 *	05/08/2006 - Added verify_clockres to test whether or not clock
 *	             resolution is within the acceptable range (<= 200us).
 */

#include <math.h>
#include <time.h>
#include <dlfcn.h>
#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/resource.h>

#ifndef VERSION
#define VERSION			".dev"
#endif

/* test error codes */
#define FAIL_MEMLOCK		0x0001
#define FAIL_SCHED		0x0002
#define FAIL_ROBUST_MUTEX	0x0004
#define FAIL_PREEMPT_RT		0x0008
#define FAIL_HRTIMER_RES	0x0010
#define FAIL_CLOCK_RES		0x0020
#define FAIL_MLOCKALL		0x0040

/* internal error codes */
#define FAIL_BOOT_ID_MISMATCH	0x0100
#define FAIL_NO_CACHE		0x0200

#define OPT_VERBOSE		0x0001
#define OPT_FORCE		0x0002

#define RT_F			"/var/cache/rtcheck"
#define PROC_F			"/proc/sys/kernel/random/boot_id"
#define PROC_ROOT		"/proc"
#define BOOT_ID_LEN		36

#define US_PER_SEC		1000000
#define NS_PER_US		1000

#define PRINT_RESULT(MSG, RET) \
if (flags & OPT_VERBOSE) printf("%s: %s\n", MSG, (RET) ? "failed" : "ok")

#define PRINTF_RESULT(FORMAT, RET, ...) \
if (flags & OPT_VERBOSE) printf(FORMAT": %s\n", __VA_ARGS__, (RET) ? "failed" : "ok")

#define PRINT(MSG) \
if (flags & OPT_VERBOSE) printf(MSG)

#define PRINTF(FORMAT, ...) \
if (flags & OPT_VERBOSE) printf(FORMAT, __VA_ARGS__)

static int rtcheck_priority = 95;
static int flags = 0;
static int prio_min, prio_max;

static void print_usage(char *progname)
{
	printf("Usage: %s [-hfVv][-p rtprio]\n", progname);
}

static void print_help(char *progname)
{
	print_usage(progname);
	printf("Test system for various real-time characteristics.\n\n");

	printf("rtcheck v%s:\n", VERSION);
	printf(" -h       Print help message\n");
	printf(" -f       Ignore cached results, run all tests\n");
	printf(" -p prio  Where prio is between %d and %d (Default %d)\n",
	       prio_min, prio_max, rtcheck_priority);
	printf(" -v       Verbose mode (implies -f\n");
	printf(" -V       Version string\n");
}

/* Init: Lock all memory
 *
 * Lock all memory to avoid paging induced latencies.
 */
static uint32_t init_mlockall(void)
{
	int ret = 0;

	if (mlockall(MCL_CURRENT | MCL_FUTURE))
		ret = FAIL_MLOCKALL;

	PRINT_RESULT("  Locking all memory", ret);
	if (ret)
		PRINT("\tTests may reflect high paging latencies\n");

	return ret;
}

/* Init: Scheduler
 *
 * Setup rtcheck to run as a SCHED_FIFO task.
 *
 * Also verify user ability to run sched_setscheduler with SCHED_FIFO.  It is
 * implied that if we are able to set SCHED_FIFO, we are also able to set
 * SCHED_RR.
 */
static uint32_t init_sched(void)
{
	int ret = 0;
	struct sched_param param;

	param.sched_priority = rtcheck_priority;
	if (sched_setscheduler(0, SCHED_FIFO, &param) != 0)
		ret |= FAIL_SCHED;

	PRINT_RESULT("  Setting up real-time scheduling", ret);
	if (ret)
		PRINT("\tTests may reflect high scheduling latencies\n");

	return ret;
}

/* Test:  Memory lock
 *
 * Verify user ability to mlock ~32K of memory.
 *
 */
#define BUF_LEN 32768
static uint32_t verify_memlock(void)
{
	int ret = 0;
	int page_size;
	struct rlimit rlim;
	static char *buf;
	char *ptr;
	static int mlen = 255;
	char failure_msg[mlen];

	page_size = sysconf(_SC_PAGESIZE);

	buf = (char *)malloc(BUF_LEN+page_size-1);
	if (!buf) {
		snprintf(failure_msg, mlen, "malloc: %s", strerror(errno));
		ret = FAIL_MEMLOCK;
	} else {
		/*
		 * Make sure the pointer is page aligned and the length is a
		 * multiple of a page size.  Not strictly required by Linux,
		 * but some OS's may require this.
		 */
		ptr = (char *)((unsigned long)(buf + page_size - 1) &
		      ~(page_size - 1));

		if (mlock(ptr, BUF_LEN) < 0) {
			snprintf(failure_msg, mlen, "mlock: %s", strerror(errno));
			ret = FAIL_MEMLOCK;
		}

		if (munlock(ptr, BUF_LEN) < 0) {
			snprintf(failure_msg, mlen, "munlock: %s", strerror(errno));
			ret = FAIL_MEMLOCK;
		}

		if (getrlimit(RLIMIT_MEMLOCK, &rlim) < 0) {
			snprintf(failure_msg, mlen, "getrlimit: %s", strerror(errno));
			ret = FAIL_MEMLOCK;
		}

		if (rlim.rlim_cur != RLIM_INFINITY) {
			snprintf(failure_msg, mlen, "RLIMIT_MLOCK is: %u",
			         (unsigned int) rlim.rlim_cur);
			ret = FAIL_MEMLOCK;
		}

		free(buf);
	}

	PRINT_RESULT("  Trying to lock memory", ret);
	if (ret)
		PRINTF("\t%s\n", failure_msg);

	return ret;
}

/* Test: CONFIG_PREEMPT_RT
 *
 * Verify that the kernel was compiled with CONFIG_PREEMPT_RT=y.  If the IRQs
 * are threads and we observe interfaces such as /proc/loadavgrt, we know the
 * system is running with CONFIG_PREEMPT_RT enabled.
 */
static uint32_t verify_preempt_rt(void)
{
	int rc;
	int ret = 0;
	struct stat stats;

	rc = system("test `ps ax | grep -E -i -e 'IRQ[-/ ][1-9][0-9]*' | wc -l` -ne 0");
	if (WEXITSTATUS(rc) && stat("/proc/loadavgrt", &stats))
		ret = FAIL_PREEMPT_RT;

	PRINT_RESULT("  Checking for out-of-tree RT extensions", ret);
	if (ret)
		PRINT("\tKernel was not built with CONFIG_PREEMPT_RT=y\n");

	return ret;
}

/* Test: Robust mutexes
 *
 * Verify that the following glibc symbols exist:
 *   o pthread_mutex_init
 *   o pthread_mutexattr_getprotocol
 *   o pthread_mutexattr_setprotocol
 *
 * Verify that the following kernel symbols exist:
 *   o __rt_mutex_init
 *   o __rt_mutex_adjust_prio
 *
 * The existence of these symbols implies robust mutex support in both
 * the kernel and userspace.
 */
struct test_sym_t {
	char *sym;
	uint32_t (*do_test)(void);
};

static inline uint32_t test_pthread_mutex_init()
{
	pthread_mutex_t mutex;

	return pthread_mutex_init(&mutex, NULL);
}

static inline uint32_t test_pthread_mutexattr_getprotocol()
{
	int mutex_protocol;
	pthread_mutexattr_t mutex_attr;

	return (pthread_mutexattr_init(&mutex_attr) ||
		pthread_mutexattr_getprotocol(&mutex_attr, &mutex_protocol));
}

static inline uint32_t test_pthread_mutexattr_setprotocol()
{
	pthread_mutexattr_t mutex_attr;

	return (pthread_mutexattr_init(&mutex_attr) ||
		pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT));
}

static uint32_t verify_robust_mutex(void)
{
	int ret = 0;
	struct test_sym_t *next;
	struct test_sym_t syms[] = { {"pthread_mutex_init", test_pthread_mutex_init},
				     {"pthread_mutexattr_getprotocol", test_pthread_mutexattr_getprotocol},
				     {"pthread_mutexattr_setprotocol", test_pthread_mutexattr_setprotocol},
				     {0, 0}, };
	void *dl;
	void *sym_addr;
	static int mlen = 255;
	char failure_msg[mlen];

	for (next = syms; next->sym != NULL; next++) {
		dl = dlopen(NULL, RTLD_NOW);
		sym_addr = dlsym(dl, next->sym);
		dlclose(dl);
		if (!sym_addr) {
			snprintf(failure_msg, mlen,
			         "Checking for %s: not found", next->sym);
			ret = FAIL_ROBUST_MUTEX;
			break;
		} else {
			if (next->do_test()) {
				snprintf(failure_msg, mlen, "Testing %s: %s",
				         next->sym, strerror(errno));
				ret = FAIL_ROBUST_MUTEX;
				break;
			}
		}
	}

	PRINT_RESULT("  Checking for robust (PI) mutex support", ret);
	if (ret)
		PRINTF("\t%s\n", failure_msg);

	return ret;
}

/* Test: High resolution timers
 *
 * Verify the system can use high resolution timers.
 */
#define HRTIMER_MAX_RES	100
static uint32_t verify_timer_res(void)
{
	int ret = 0;
	struct timespec before;
	struct timespec after;
	struct timespec sleep = {0, (HRTIMER_MAX_RES*NS_PER_US)/2};
	long delta_us;

	clock_gettime(CLOCK_MONOTONIC, &before);
	if (clock_nanosleep(CLOCK_MONOTONIC, 0, &sleep, NULL)) {
		fprintf(stderr,
		        "clock_nanosleep returned prematurely, aborting.\n");
		exit(-1);
	}
	clock_gettime(CLOCK_MONOTONIC, &after);
	delta_us = (after.tv_sec - before.tv_sec)*US_PER_SEC +
		(after.tv_nsec - before.tv_nsec)/NS_PER_US;

	ret = (delta_us <= HRTIMER_MAX_RES) ? 0 : FAIL_HRTIMER_RES;

	PRINTF_RESULT("  Testing for acceptable hrtimer resolution (<=%dus)",
	              ret, HRTIMER_MAX_RES);
	PRINTF("\tReporting %ldus\n", delta_us);

	return ret;
}

/* Test: Clock Resolution
 *
 * Verify clock_getres returns finer resolution than 200us.
 */
#define CLOCK_MAX_RES 200
int verify_clock_res(void)
{
	int ret = 0;
	struct timespec ts;
	static int mlen = 255;
	char detail_msg[mlen];

	if (clock_getres(CLOCK_REALTIME, &ts) < 0) {
		snprintf(detail_msg, mlen, "clock_getres: %s\n",
		         strerror(errno));
		ret = FAIL_CLOCK_RES;
	} else {
		ret = (ts.tv_sec == 0 && ts.tv_nsec / NS_PER_US <= CLOCK_MAX_RES) ? 0 : FAIL_CLOCK_RES;
		if (ts.tv_nsec < NS_PER_US)
			snprintf(detail_msg, mlen, "Reporting %ldns", ts.tv_nsec);
		else
			snprintf(detail_msg, mlen, "Reporting %ldus", (ts.tv_nsec / NS_PER_US));
	}

	PRINTF_RESULT("  Testing for acceptable clock resolution (<=%dus)",
	              ret, CLOCK_MAX_RES);
	PRINTF("\t%s\n", detail_msg);

	return ret;
}

static int get_boot_id(char *boot_id)
{
	int ret = 0;
	FILE *proc_f;

	boot_id[0] = 0;
	proc_f = fopen(PROC_F, "r");
	if (!proc_f) {
		ret = errno;
	} else {
		fread(boot_id, sizeof(char), BOOT_ID_LEN, proc_f);
		if (ferror(proc_f))
			ret = errno;
		boot_id[BOOT_ID_LEN] = 0;
		fclose(proc_f);
	}

	PRINTF_RESULT("  Looking up boot_id (%s)", ret,
	              strcmp(boot_id, "") ? boot_id : "ERROR");
	if (ret) {
		PRINTF("\t%s\n", strerror(ret));
		PRINT("\tSystem test results will not be cached\n");
	}

	return ret;
}

static int fetch_rtcheck_status(char *boot_id)
{
	int ret = 0;
	FILE *rt_f = NULL;
	char rt_boot_id[BOOT_ID_LEN+1];
	char rtcheck_status[3];

	rt_f = fopen(RT_F, "r");
	if (rt_f) {
		// Seek passed "boot_id" key
		fseek(rt_f, 8, SEEK_CUR);

		fread(rt_boot_id, sizeof(char), BOOT_ID_LEN, rt_f);
		if (!ferror(rt_f)) {
			rt_boot_id[BOOT_ID_LEN] = 0;

			/* Seek passed "rtcheck" key */
			fseek(rt_f, 9, SEEK_CUR);
			fgets(rtcheck_status, 3, rt_f);

			fclose(rt_f);

			if (!strcmp(rt_boot_id, boot_id)) {
				ret = atoi(rtcheck_status);
			} else {
				ret = FAIL_BOOT_ID_MISMATCH;
			}
		}
	} else {
		if (errno == ENOENT)
			ret = FAIL_NO_CACHE;
		/* check other error conditions? */
	}

	/* No prints here since this routine never runs with verbose on */

	return ret;
}

static void store_rtcheck_status(char *boot_id, uint32_t result)
{
	FILE *rt_f;
	int ret = 0;
	int mlen = 255;
	char failure_msg[mlen];

	rt_f = fopen(RT_F, "w");
	if (!rt_f) {
		snprintf(failure_msg, mlen, "%s could not be written to: %s",
		         RT_F, strerror(errno));
		ret = -1;
	} else {
		fprintf(rt_f, "boot_id=%s\nrtcheck=%u\n", boot_id, result);
		fclose(rt_f);
	}

	PRINTF_RESULT("  Caching results in %s", ret, RT_F);
	if (ret)
		PRINTF("\t%s\n", failure_msg);
}

static uint32_t do_system_tests(void)
{
	uint32_t ret = 0;

	ret |= verify_preempt_rt();
	ret |= verify_robust_mutex();
	ret |= verify_timer_res();
	ret |= verify_clock_res();

	return ret;
}

int main(int argc, char **argv)
{
	int opt;
	uint32_t ret = 0;
	char boot_id[BOOT_ID_LEN+1];
	struct stat stats;
	int opt_prio;
	int result;

	prio_min = sched_get_priority_min(SCHED_FIFO);
	if (prio_min < 0) {
		perror("Failed to determine minimum SCHED_FIFO priority");
		exit(-1);
	}
	prio_max = sched_get_priority_max(SCHED_FIFO);
	if (prio_max < 0) {
		perror("Failed to determine maximum SCHED_FIFO priority");
		exit(-1);
	}

	while ((opt = getopt(argc, argv, "fhp:Vv")) != EOF) {
		switch (opt) {
		case 'f':
			flags |= OPT_FORCE;
			break;
		case 'h':
			print_help(argv[0]);
			exit(0);
		case 'p':
			opt_prio = atoi(optarg);
			if (opt_prio < prio_min || opt_prio > prio_max) {
				fprintf(stderr, "Priority out of range: %d\n",
				        opt_prio);
				print_usage(argv[0]);
				exit(-1);
			}
			rtcheck_priority = opt_prio;
			break;
		case 'v':
			flags |= OPT_VERBOSE;
			flags |= OPT_FORCE;	/* Can't display all the
						   output with cached values */
			break;
		case 'V':
			printf("rtcheck v%s:\n", VERSION);
			exit(0);
		default:
			print_usage(argv[0]);
			exit(-1);
		}
	}

	if (optind > argc) {
		print_usage(argv[0]);
		exit(-1);
	}

	if (stat(PROC_ROOT, &stats)) {
		fprintf(stderr, "The /proc interface must be enabled to make some "
		       "determinations regarding real-time capabilities of "
		       "this system.\n");
		exit(-1);
	}

	PRINTF("RTCheck %s - Linux Real-Time Environment Checker\n", VERSION);
	PRINT("---------------------------------------------------\n");

	PRINT("RTCheck Initialization:\n");
	ret |= init_mlockall();	/* Avoid paging induced latencies */
	ret |= init_sched();	/* Run prior to latency sensitive tests */

	PRINT("\nSystem Tests:\n");
	if (get_boot_id(boot_id) < 0 || flags & OPT_FORCE) {
		result = do_system_tests();
		store_rtcheck_status(boot_id, result);
	} else {
		result = fetch_rtcheck_status(boot_id);
		/* Re-run if the boot_id doesn't match */
		if (result == FAIL_BOOT_ID_MISMATCH ||
		    result == FAIL_NO_CACHE) {
			result = do_system_tests();
			store_rtcheck_status(boot_id, result);
		}
	}
	ret |= result;

	PRINT("\nUser Permission Tests:\n");
	ret |= verify_memlock();

	if (ret) {
		PRINTF("\nSome tests failed, exiting with status: %d\n", ret);
	} else {
		PRINT("\nAll tests passed\n");
	}

	exit(ret);
}
