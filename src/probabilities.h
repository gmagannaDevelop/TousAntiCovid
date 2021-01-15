#ifndef __asd9j2348s_PROBABILITIES_H
  #define __asd9j2348s_PROBABILITIES_H
  
/*# Definition of simulation probabilities  */
    #define A_INIT_LAMBDA 0.20
    #define A_INIT_DOCTOR 0.10 
    #define A_INIT_VIRUS 0.05 
    #define P_INIT_LAMBDA A_INIT_LAMBDA
    #define P_INIT_DOCTOR A_INIT_DOCTOR / (1.0 - A_INIT_LAMBDA)
    #define P_INIT_VIRUS A_INIT_VIRUS / ((1.0 - A_INIT_LAMBDA )*(1.0 - A_INIT_DOCTOR))
    
    #define P_ASYMPTOMATIC 1.0/3.0                                                  
    #define P_SYMPTOMATIC 1.0 - P_ASYMPTOMATIC
    #define P_SNEEZE 0.25

    #define VIRULENCE 1.0/10.0
    #define MORTALITY VIRULENCE

    #define P_MOVE 0.70
    #define P_STAY 1.0 - P_MOVE

    #define P_MOVE_MOMENTUM 3.0/4.0
    #define P_MOVE_RANDOM 1.0 - P_MOVE_MOMENTUM

    #define P_STAY_ON_COLLISION 0.5
    #define P_MOVE_ON_COLLISION 1.0 - P_STAY_ON_COLLISION

/*# Function protoypes : */
  
    /*Perform a bernoulli trial (binomial experiment)
      with success probability `p`.
      Returns 
        TRUE (1, with a probability `p`)
        FALSE (O, with a probability `1-p`)        */
    int bernoulli_trial(gsl_rng **randgen, double p);

    int correct_posterior_probs(double *p_lambda, double *p_doctor, double *p_virus);

#endif
