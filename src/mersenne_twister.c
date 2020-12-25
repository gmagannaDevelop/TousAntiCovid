
#include "libraries.h"

double draw_randnum_01(gsl_rng **randgen)
{
  return gsl_rng_uniform(*randgen);
}

int draw_randint_0n(gsl_rng **randgen, int n)
{
  return (int)gsl_rng_uniform_int(*randgen, (unsigned long int)n);
}

double draw_randum_interval(
  gsl_rng **randgen, double a, double b
)
{
  return a + (b - a) * draw_randnum_01(randgen);
}

int initialize_randgen(gsl_rng **randgen, int verbosity)
{
    unsigned long int seed;
    
    *randgen = gsl_rng_alloc(gsl_rng_mt19937);
    if(*randgen == NULL){
      printf("\n\nGSL could not allocate random number generator.\n\n");
      exit(0); 
    }
    
    if(verbosity){
      printf("\n\nGenerator type: %s\n", gsl_rng_name(*randgen));
      printf("Fetching seed from /dev/random.\n");
    }
    
    seed = FIXED_RNG_SEED;
    /* dev_random_ulongint(verbosity);
    */
    gsl_rng_set(*randgen, seed);
    
    if(verbosity){
      printf("Seed = %ld\n", seed);
      printf("First value = %e\n\n", gsl_rng_uniform(*randgen));
    }
    
    return(1);
}
