/*
 * Example Code for the generation of uniform
 * random numbers from the interval [0, 1],
 * using the Mersenne Twister quasi random number
 * generator from the Gnu Scientific Library (GSL),
 * and an 8-byte seed from /dev/random.
 *
 * M. Heinen, November 2017
 * marco@marco-heinen.de
 *
 */ 

#include "libraries.h"

#define VERBOSITY 1

#define MIN 0.0
#define MAX 1.0


int main(int argc, char **argv)
{
int n, N;
double rand;
gsl_rng *randgen;

N = parse_commandline(argc, argv);

initialize_randgen(&randgen, VERBOSITY);

printf("\n#\n# Mersenne Twister Quasi random numbers after\n"\
       "# initialization with /dev/random:\n#\n");

for(n = 0; n < N; n++)
  {
  rand = MIN + (MAX-MIN)*draw_randnum_01(&randgen);
  printf("%.16f\n", rand);
  }

return(1);
}
