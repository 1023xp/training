/*
 * Shared lib to test different access patterns and sharing
 *
 * (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license
 */
#include "sharedarray.h"
#include <stdio.h>

/* Uninitialised stuff will go into .bss and get special zero-page
 * treatment */
BigArr uninit_donttouch;
BigArr uninit_readme;
BigArr uninit_writeme;
BigArr uninit_readhalf;
BigArr uninit_writehalf;

BigArr donttouch = { 1 };
BigArr readme = { 2 };
BigArr writeme = { 3 };
BigArr readhalf = { 4 };
BigArr writehalf = { 5 };

int do_shared_array_work(void)
{
	int i;
	int sum = 0;
	
	for (i = 0; i < ARRAY_SIZE; ++i) {
		sum += readme[i];
		sum += uninit_readme[i];
		writeme[i] = i;
		uninit_writeme[i] = i;
	}

	for (i = 0; i < ARRAY_SIZE / 2; ++i) {
		sum += uninit_readhalf[i];
		sum += readhalf[i];
		writehalf[i] = i;
		uninit_writehalf[i] = i;
	}

	return 0;
}
