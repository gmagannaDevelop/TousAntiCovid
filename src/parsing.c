#include "libraries.h"



void parse_commandline(
  int argc, char **argv, 
  int *N, int *M, int *save_output
){

if(argc!=4){ explain_useage_exit(argv[0]); }

if(1 != sscanf(argv[1], "%d", N) ){ explain_useage_exit(argv[0]); }

if(1 != sscanf(argv[2], "%d", M) ){ explain_useage_exit(argv[0]); }

if(1 != sscanf(argv[3], "%d", save_output) ){ explain_useage_exit(argv[0]); }

if( /* Check valid dimensions */
    (*N < 2) || (*M < 2) ||
    /* Check valid save_output */
    ((*save_output > 1) || (*save_output < 0))
){ explain_useage_exit(argv[0]); }

}





void explain_useage_exit(char *myname)
{
printf("\n\nUseage:\n\n%s N M graphics\n\n"\
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
       "automatically converted to gif by system calls.\n\n"\
       "Parameters N and M should be integers greater than two.\n\n\n");
exit(EXIT_FAILURE);
}

