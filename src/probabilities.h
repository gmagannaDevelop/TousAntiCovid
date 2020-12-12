#ifndef __asd9j2348s_PROBABILITIES_H
  #define __asd9j2348s_PROBABILITIES_H
  
  /* Definition of simulation probabilities  */
  
  /* Function protoypes : */
  
  /* 
    Perform a bernoulli trial (binomial experiment)
    with success probability `p`.
    Returns 
      TRUE (1, with a probability `p`)
      FALSE (O, with a probability `1-p`)
  */
  int bernoulli_trial(gsl_rng **randgen, double p);

#endif