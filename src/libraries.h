#ifndef __asl234n0a2__LIBRARIES_H
  #define __asl234n0a2__LIBRARIES_H
  
  /* Standard libraries : */
  #include <stdio.h>
  #include <stdlib.h>
  #include <signal.h>
  #include <time.h>
  #include <math.h>
    /* wtf ? */
    #include <fcntl.h>
    #include <unistd.h>
    #include <limits.h>

  /* Sleeping-related */
  #include <time.h>
  #include <errno.h>    

  // parallel computing
  #include <omp.h>

  /* GNU Scientific library */
  #include <gsl/gsl_rng.h>

  /* Simple Direct Media layer (SDL) graphics: */
  #include <SDL/SDL.h>

  /* Locally defined */
    /* Headers : */
    #include "dynamics.h"
    #include "simulation.h"
    #include "display.h"
    #include "parsing.h"
    #include "aux_math.h"
    #include "dev_random.h"
    #include "mersenne_twister.h"
    #include "singly_linked_list.h"
    #include "datastructures.h"
    #include "SDL_datastructure.h"
    #include "visualization.h"

    /* Constants and macros : */
    #include "constants.h"
    #include "probabilities.h"

#endif
