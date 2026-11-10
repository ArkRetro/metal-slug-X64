/* LD_PRELOAD shim for MAME (docs/TAS.md): the wall clock (time(),
 * gettimeofday(), clock_gettime(CLOCK_REALTIME)) is fixed at MSX_FIXED_TIME
 * (Unix seconds; default 1262304000 = 2010-01-01T00:00:00 UTC), so MAME's
 * base time, from which the uPD4990A RTC registers are set and re-read at
 * every 1 s tick, is the movie's InitialTime (BizHawk's base-time callback).
 * The monotonic clocks used for throttling are left alone.
 * Build: gcc -shared -fPIC -O2 -o fixed_time.so fixed_time.c -ldl */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

static time_t fixed(void)
{
    const char *s = getenv("MSX_FIXED_TIME");
    return s ? (time_t)strtoll(s, NULL, 10) : (time_t)1262304000;
}

time_t time(time_t *t)
{
    time_t v = fixed();
    if (t) *t = v;
    return v;
}

int gettimeofday(struct timeval *tv, void *tz)
{
    (void)tz;
    if (tv) { tv->tv_sec = fixed(); tv->tv_usec = 0; }
    return 0;
}

int clock_gettime(clockid_t id, struct timespec *ts)
{
    static int (*real)(clockid_t, struct timespec *);
    if (id == CLOCK_REALTIME || id == CLOCK_REALTIME_COARSE) {
        ts->tv_sec = fixed(); ts->tv_nsec = 0;
        return 0;
    }
    if (!real) real = (int (*)(clockid_t, struct timespec *))dlsym(RTLD_NEXT, "clock_gettime");
    return real(id, ts);
}
