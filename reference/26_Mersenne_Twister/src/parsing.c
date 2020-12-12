#include "libraries.h"


int parse_commandline(int argc, char **argv)
{
int N = 0;

if(argc!=2)
  {
  explain_useage_exit(argv[0]);
  }

if(1 != sscanf(argv[1], "%d", &N) )
  {
  explain_useage_exit(argv[0]);
  }

if(N < 1)
  {
  explain_useage_exit(argv[0]);
  }

return(N);
}



void explain_useage_exit(char *myname)
{
printf("\n\nUseage:\n\n%s  N\n\nwill output N quasi random numbers\n"\
       "that are generated with the Mersenne Twister algorithm\n"\
       "from the Gnu Scientific Library (GSL). The generator is\n"\
       "initialized with a %lu byte seed from /dev/random.\n\n"\
       "N must be a positive integer.\n\n", myname, sizeof(unsigned long int));
exit(0);
}
