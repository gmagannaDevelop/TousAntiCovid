#include "libraries.h"



unsigned long int dev_random_ulongint(int verbosity)
{
unsigned int first_random_uint, second_random_uint;
unsigned long int random_ulong;
int devrand;
ssize_t bytes_read;
//double rand_null_one, dummy;

if( sizeof(unsigned long int) != 2*sizeof(unsigned int) )
  {
  printf("\n\nExpected sizeof(unsigned long int) to be 2*sizeof(unsigned int),\n"\
         "but found the following:\n\n"\
         "sizeof(unsigned long int) = %ld\n"\
         "sizeof(unsigned int) = %ld\n\n\n",\
         sizeof(unsigned long int), sizeof(unsigned int));
  exit(0); 
  }

/* Get the first random unsigned int from /dev/random: */ 
do
  {
  devrand = open("/dev/random", O_RDONLY);
  bytes_read = read(devrand, &first_random_uint, sizeof(unsigned int));
  if(verbosity)
    {
    printf("  Requested %ld bytes and got %ld bytes from /dev/random ...\n",\
           sizeof(unsigned int), bytes_read);
    }
  close(devrand);
  }
while(bytes_read < sizeof(unsigned int));


/* Get the second random unsigned int from /dev/random: */ 
do
  {
  devrand = open("/dev/random", O_RDONLY);
  bytes_read = read(devrand, &second_random_uint, sizeof(unsigned int));
  if(verbosity)
    {
    printf("  Requested %ld bytes and got %ld bytes from /dev/random ...\n",\
           sizeof(unsigned int), bytes_read);
    }
  close(devrand);
  }
while(bytes_read < sizeof(unsigned int));

/* Combine two random unsigned integers into one random unsigned long int: */
random_ulong = (unsigned long int)first_random_uint;
random_ulong = random_ulong << (8 * sizeof(unsigned int)); 
random_ulong += (unsigned long int)second_random_uint;

return(random_ulong);
}
       



unsigned long int ulongint_power(unsigned long int n, unsigned long int power)
{
if(power == 0)
  {
  return(1);
  }
return(n * ulongint_power(n, power-1));
}
