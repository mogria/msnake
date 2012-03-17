#ifndef _CURRENT_UTC_TIME_H
#define _CURRENT_UTC_TIME_H

/**
 * mac version: jbenet (https://gist.github.com/1087739)
 * windows version: Carl Staelin (http://stackoverflow.com/questions/5404277/porting-clock-gettime-to-windows)
 */

#include <time.h>
#include <sys/time.h>
#include <stdio.h>

// mac ?
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

// windows?
#ifdef _WIN32

// we need windows.h to implement clock_gettime on windows
#include <windows.h>

// this may not be defined on windows
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 15 /* most random value evarr! */
#endif

// the timespec structure is not available on windows
#define timespec timeval

// a little helper function
LARGE_INTEGER getFILETIMEoffset() {
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}

int clock_gettime(int X, struct timeval *tv);

int clock_gettime(int X, struct timeval *tv) {
    LARGE_INTEGER           t;
    FILETIME                f;
    double                  microseconds;
    static LARGE_INTEGER    offset;
    static double           frequencyToMicroseconds;
    static int              initialized = 0;
    static BOOL             usePerformanceCounter = 0;

    if (!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        } else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = microseconds;
    tv->tv_sec = t.QuadPart / 1000000;
    tv->tv_usec = (t.QuadPart % 1000000) * 1000;
    return (0);
}
#endif /* _WIN32 */


// the portable function for clock_gettime ;-)
void current_utc_time(struct timespec *ts) {
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, ts);
}
#endif

#endif /* _CURRENT_UTC_TIME_H */
