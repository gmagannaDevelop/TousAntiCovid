

#include "libraries.h"



int bernoulli_trial(gsl_rng **randgen, double p)
{/* 
    Perform a bernoulli trial (binomial experiment)
    with success probability `p`.
    Returns 
      TRUE (1, with a probability `p`)
      FALSE (O, with a probability `1-p`)
  */
    double q;

    q = 1.0 - p;
    if (draw_randnum_01(randgen) > q){
        return TRUE;
    }
    else {
        return FALSE;
    }
     
}