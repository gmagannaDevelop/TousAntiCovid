

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

int correct_posterior_probs(
    double *p_lambda, double *p_doctor, double *p_virus
){
   double lambda, doctor, virus;

   lambda = *p_lambda;
   doctor = *p_doctor;
   virus = *p_virus;

   *p_doctor = doctor / (1.0 - lambda);
   *p_virus = virus / ((1.0 - lambda)*(1.0 - doctor));

    //printf("Corrected posteriors:\n pd = %lf, pv = %lf\n", *p_doctor, *p_virus);
    if ((*p_doctor > 1.0) || (*p_virus > 1.0)){
        printf("!!! ERROR !!!\n");
        printf("Invalid corrected posterior probabilities found :\n");
        printf("p_doctor : %.5lf, p_virus : %.5lf\n", *p_doctor, *p_virus);
        exit(EXIT_FAILURE);
    }
    else {
        return TRUE;
    }
}