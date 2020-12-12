#ifndef __asl234n0a2__LIBRARIES_H
  #define __asl234n0a2__LIBRARIES_H
  
  /* Standard libraries : */
  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>
    /* wtf ? */
    #include <fcntl.h>
    #include <unistd.h>
    #include <limits.h>

  /* GNU Scientific library */
  #include <gsl/gsl_rng.h>

  /* Graphics : */
  #include <SDL2/SDL.h>
  #include <SDL2/SDL_image.h>

  /* Locally defined */
    /* Headers : */
    #include "aux_math.h"
    #include "dev_random.h"
    #include "mersenne_twister.h"
    #include "random_numbers.h"

    /* Constants and macros : */
    #include "constants.h"
    #include "probabilities.h"

#endif
