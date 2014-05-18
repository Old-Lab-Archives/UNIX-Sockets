/* This program which implements the subtraction operation between two timeval structures 
and it finally stores the result in the first structure. */
#include "meow.h"
void tv_sub(struct timeval *out, struct timeval *in)
{
	if((out->tv_usec-=in->tv_usec) < 0)
	{
		/* out-=in; */
		--out->tv_sec;
		out->tv_usec+=1000000;
	}
	out->tv_sec-=in->tv_sec;
}
