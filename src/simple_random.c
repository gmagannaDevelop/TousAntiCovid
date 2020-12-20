/*
 * Some very simple quasi-random number generation functions,
 * using the rand() function from stdlib. Don't use these
 * functions if you need high quality random numbers.
 *
 */
#include "libraries.h"


void initialize_rand(void)
{
srand( (unsigned int)time(NULL) );
}



char rand_zero_one(void)
{
/* Returns 0 or 1, with an almost 50:50 probability. */
return( (char)(rand() % 2) );
}



unsigned int rand_zero_n(unsigned int n)
{
/* Return a quasi random number from
 * the interval [0, n-1].
 *
 */
return( (unsigned int)(rand() % n) );
}
