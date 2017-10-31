#pragma once

//from: http://stackoverflow.com/questions/2494356/how-to-use-gettimeofday-or-something-equivalent-with-visual-studio-c-2008

#ifndef SYS_TIME_EXTENDED_H_
#define SYS_TIME_EXTENDED_H_

//#include "stdafx.h"
#include <time.h>
#include <windows.h> 

struct timezone2
{
	__int64  tz_minuteswest; /* minutes W of Greenwich */
	bool  tz_dsttime;     /* type of dst correction */
};

struct timeval2 {
	__int64    tv_sec;         /* seconds */
	__int64    tv_usec;        /* microseconds */
};

int gettimeofday2(struct timeval2 *tv/*in*/, struct timezone2 *tz/*in*/);

#endif