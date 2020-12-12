#ifndef __asd9j2348s_PROBABILITIES_H
  #define __asd9j2348s_PROBABILITIES_H
  
/*# Definition of simulation probabilities  */
    #define P_INIT_LAMBDA 0.20
    #define P_INIT_DOCTOR 0.10
    #define P_INIT_VIRUS 0.05
    
    #define P_ASYMPTOMATIC 2.0/3.0                                                  
    /* Given the dicotomy of the events, 
       there may not be a need for
       P_SYMPTOMATIC
    */
    #define P_SYMPTOMATIC 1.0 - P_ASYMPTOMATIC

    #define VIRULENCE 1.0/10.0
    #define MORTALITY VIRULENCE
  
/*# Function protoypes : */
  
    /* 
      Perform a bernoulli trial (binomial experiment)
      with success probability `p`.
      Returns 
        TRUE (1, with a probability `p`)
        FALSE (O, with a probability `1-p`)
    */
    int bernoulli_trial(gsl_rng **randgen, double p);

#endif