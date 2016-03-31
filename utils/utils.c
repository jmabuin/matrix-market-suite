#include "utils.h"

/*********
 * Timer *
 *********/

double cputime()
{
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
	return r.ru_utime.tv_sec + r.ru_stime.tv_sec + 1e-6 * (r.ru_utime.tv_usec + r.ru_stime.tv_usec);
}

double systemtime()
{
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
	return r.ru_stime.tv_sec + 1e-6 * (r.ru_stime.tv_usec);
}

double usertime()
{
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
	return r.ru_utime.tv_sec + 1e-6 * (r.ru_utime.tv_usec);
}

double realtime()
{
	struct timeval tp;
	struct timezone tzp;
	gettimeofday(&tp, &tzp);
	return tp.tv_sec + tp.tv_usec * 1e-6;
}
