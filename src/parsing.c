#include "libraries.h"



void parse_commandline(
  int argc, char **argv, 
  int *N, int *M, 
  double *p_lambda, double *p_doctor, double *p_virus,
  int *max_sim_steps
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

    if(1 != sscanf(argv[6], "%d", max_sim_steps) ){ explain_useage_exit(argv[0]); }

    if( // Check valid dimensions 
        (*N < 2) || (*M < 2) ||
        // Check valid probabilities :
        (*p_lambda > 1.0) || (*p_lambda < 0.0) || are_close(*p_lambda, 1.0) ||
        (*p_doctor > 1.0) || (*p_doctor < 0.0) || are_close(*p_doctor, 1.0) ||
        (*p_virus  > 1.0) || (*p_virus  < 0.0) || are_close(*p_virus,  1.0) ||
        (*p_lambda + *p_doctor > 1.0) || (total_p < 0.0) ||
        // Check valid save_output
        ((*max_sim_steps < 10))
    ){ 
      printf("N = %d, M = %d, pλ = %.3lf, pd = %.3lf, pv = %.3lf, pλ + pd = %.3lf, max_sim_steps = %d\n",
        *N, *M, *p_lambda, *p_doctor, *p_virus, total_p, *max_sim_steps
      );
      explain_useage_exit(argv[0]); 
    }

}





void explain_useage_exit(char *myname)
{
    printf("\nUseage:\n\n%s N M pλ pd pv max_sim_steps\n\n"\
           "N integer >= 2\n"\
           "M integer >= 2\n"\
           "pλ ε [0, 1.0[ := lambda probability ~ population density in simulation matrix \n"\
           "pd ε [0, 1.0[ := doctor probability ~ idem. for medical personel density\n"\
           "pv ε [0, 1.0[ := virus probability  ~ idem. for viral particles.\n"\
           "max_sim_steps := integer >= 10\n"\
           "\nWill run a montecarlo simulation of a viral epidemic\n"\
           "in a grid of N lines and M columns with periodic boundaries,\n"\
           "meaning that a person going out the top of the grid will\n"\
           "come back in from the bottom, etc.\n", myname);
    printf("\n Colour codes :\n"
           " *  blue   for \"bonhomme lambda\" (civilians)\n"\
           " *  green  for \"soignants\" (medical doctors)\n"\
           " *  orange for asymptomatic infections      \n"\
           " *  red    for symptomatic (severe) cases   \n"\
           " Viral particles are represented with smaller red dots.\n");
    printf("\nThis simulation leverages the following libraries :\n"\
           "  SDL, for graphics output (https://www.libsdl.org/) \n"\
           "  GSL, for pseudo-random number generation (https://www.gnu.org/software/gsl/) \n");
    exit(EXIT_FAILURE);
}

