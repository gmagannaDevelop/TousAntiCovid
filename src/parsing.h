#ifndef __34gd67_PARSING_H
#define __34gd67_PARSING_H

    void parse_commandline(
        int argc, char **argv, 
        int *N, int *M, 
        double *p_lambda, double *p_doctor, double *p_virus,
        int *save_output
    );

    void explain_useage_exit(char *myname);

#endif
