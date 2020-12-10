#include "libraries.h"


int parse_commandline(int *nodes, int *max_random, int argc, char **argv)
{
if(argc!=3)
  {
  explain_useage_exit(argv[0]);
  }

if(1 != sscanf(argv[1], "%d", nodes))
  {
  explain_useage_exit(argv[0]);
  }

if(1 != sscanf(argv[2], "%d", max_random))
  {
  explain_useage_exit(argv[0]);
  }

if(*nodes < 1)
  {
  explain_useage_exit(argv[0]);
  }

if(*max_random < 1)
  {
  explain_useage_exit(argv[0]);
  }

return(1);
}
  


void explain_useage_exit(char *myname)
{
printf("\n\nUseage:\n\n%s n maxrand\n\n"\
       "Will create a binary tree with n elements\n"\
       "that are randomly drawn from the interval\n"\
       "[-maxrand, maxrand].\n\n"\
       "Both arguments must be positive integers.\n\n",\
       myname);
exit(0); 
}
