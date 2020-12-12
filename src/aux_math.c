
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
