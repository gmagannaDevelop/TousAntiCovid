
#include "libraries.h"

int are_close(double a, double b)
{
    double abs_delta;
    abs_delta = fabs(a - b);
    if (abs_delta < TINY) {
      return TRUE;
    } 
    else {
      return FALSE;
    }
}

/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
