#include "libraries.h"



void parse_commandline(
  int argc, char **argv, 
  int *N, int *M, 
  double *p_lambda, double *p_doctor, double *p_virus,
  int *save_output
){
    double total_p;

    if(argc!=7){ explain_useage_exit(argv[0]); }

    // Simulation dimensions :
    if(1 != sscanf(argv[1], "%d", N) ){ explain_useage_exit(argv[0]); }
    if(1 != sscanf(argv[2], "%d", M) ){ explain_useage_exit(argv[0]); }

    // Probabilities :
    if(1 != sscanf(argv[3], "%lf", p_lambda) ){ explain_useage_exit(argv[0]); }
    if(1 != sscanf(argv[4], "%lf", p_doctor) ){ explain_useage_exit(argv[0]); }
    if(1 != sscanf(argv[5], "%lf", p_virus)  ){ explain_useage_exit(argv[0]); }
    total_p = *p_lambda + *p_doctor + *p_virus;

    if(1 != sscanf(argv[6], "%d", save_output) ){ explain_useage_exit(argv[0]); }

    if( // Check valid dimensions 
        (*N < 2) || (*M < 2) ||
        // Check valid probabilities :
        (*p_lambda > 1.0) || (*p_lambda < 0.0) || are_close(*p_lambda, 1.0) ||
        (*p_doctor > 1.0) || (*p_doctor < 0.0) || are_close(*p_doctor, 1.0) ||
        (*p_virus  > 1.0) || (*p_virus  < 0.0) || are_close(*p_virus,  1.0) ||
        (*p_lambda + *p_doctor > 1.0) || (total_p < 0.0) ||
        // Check valid save_output
        ((*save_output > 1) || (*save_output < 0))
    ){ 
      printf("N = %d, M = %d, pλ = %.3lf, pd = %.3lf, pv = %.3lf, pλ + pd = %.3lf, save_output = %d\n",
        *N, *M, *p_lambda, *p_doctor, *p_virus, total_p, *save_output
      );
      explain_useage_exit(argv[0]); 
    }

}





void explain_useage_exit(char *myname)
{
    printf("\n\nUseage:\n\n%s N M pλ pd pv graphics\n\n"\
           "N integer >= 2\n"\
           "M integer >= 2\n"\
           "pλ ε [0, 1.0[ \n"\
           "pd ε [0, 1.0[ \n"\
           "pv ε [0, 1.0[ \n"\
           "graphics := either 1 or 0\n"\
           "will run a viral epidemic simulation\n"\
           "in a grid of N lines and M columns\n\n"\
           "The grid has periodic boundaries, meaning that a person going out\n"\
           "of the top of the grid will come back in from the bottom\n"\
           "analogously with left and right, or any diagonal direction.\n"\
           ".\n"\
           ".\n"\
           ".\n", myname);
    printf("This simulation will produce graphical output\n"\
           "Graphical output uses two methods:\n"\
           "First, the SDL library (libsdlorg) for on-the-fly graphics window output, and\n"\
           "second, ppm picture output (if save_graphics = 1),\n"\
           "automatically converted to gif by system calls.\n\n");
    exit(EXIT_FAILURE);
}

