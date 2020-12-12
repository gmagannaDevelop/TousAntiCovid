#ifndef __fh48xx37_MERSENNETWISTER_H
#define __fh48xx37_MERSENNETWISTER_H


double draw_randnum_01(gsl_rng **randgen);

int initialize_randgen(gsl_rng **randgen, int verbosity);


#endif
