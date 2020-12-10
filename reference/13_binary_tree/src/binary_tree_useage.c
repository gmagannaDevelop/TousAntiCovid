/*
 * Examples for the useage of a
 * binary tree, as defined in
 * 'binary_tree.c' and
 * 'binary_tree.h'.
 *
 * M. Heinen, August 2017
 * marco@marco-heinen.de
 *
 */

#include "libraries.h"

#define MiB 1048576 /* This is 2^20, the number of bytes in a MiB. */ 
#define MiBs_TO_USE 1000
#define REPEATS 0


int main(int argc, char **argv)
{
int nodes = 0, node = 0, repeat = 0;
int content = 0, max_random = 0;
int ram_to_use = 0, elements = 0;
struct binary_tree *bintree;

parse_commandline(&nodes, &max_random, argc, argv);

if(REPEATS)
  {
  ram_to_use = MiBs_TO_USE * MiB;
  printf("\n\nWill use approx. %.0f MiB for binary tree in memory leak test run\n",\
         ((float)ram_to_use) / ((float)MiB));
  elements = ram_to_use / (3 * sizeof(struct binary_tree));
  printf("with %d nodes.\n\n", elements);
  }

/* initialize_rand(); */

/* Binary tree initialization: */
printf("\n\nInitializing a binary tree...");
allocate_and_initialize_bintree(&bintree);
printf(" Done.\n\n");

printf("\nStarting memory leak test run.\n");
for(repeat = 0; repeat < REPEATS; repeat++)
  {
  printf("REPEAT %d of %d:\n", repeat+1, REPEATS); 
  printf("Filling the binary tree with lots of random numbers.\n");
  for(node = 0; node < elements; node++)
    {
    content = rand_zero_n(2*elements+1) - elements;
    extend_bintree(bintree, content);
    }
  printf("Emptying the binary tree.\n");
  empty_bintree(bintree);
  }
printf("Memory leak test run finished.\n");

/* Fill the binary tree with random content: */
for(node = 0; node < nodes; node++)
  {
  content = rand_zero_n(2*max_random+1) - max_random;
  printf("Saving number %4d in binary tree.\n", content);
  extend_bintree(bintree, content);
  }

/* Determine the size of the binary tree: */
printf("\nThe binary tree contains %d elements.\n",\
       bintree_size(bintree));

/* Binary tree extremal entries: */
printf("The largest element in the binary tree is %d\n",\
       bintree_max_entry(bintree)); 
printf("The smallest element in the binary tree is %d\n",\
       bintree_min_entry(bintree)); 

/*
 * Remove the first (that is: upmost, closest to the trunk)
 * node of the binary tree that has a given content:
 *
 */
printf("\n");
content = 17;
printf("Removing the first node with content '%d'"\
       " from the tree.\n", content);
remove_node_from_bintree(bintree, content);
printf("\nThe binary tree contains %d elements.\n",\
       bintree_size(bintree));

/* Emptying the binary tree: */  
/*
printf("\nEmptying the binary tree.\n");
empty_bintree(bintree);
printf("\nThe binary tree contains %d elements.\n",\
       bintree_size(bintree));
*/

if(bintree_size(bintree))
  {
  /* Write a dot file of the graph that represents the tree: */
  printf("\nWriting 'binary_tree.dot' and a shell script.\n");
  write_bintree_dotfile(bintree, "binary_tree.dot");

  printf("\n\nFinished.\n\n"\
         "Now you can run the shellscript and look at the resulting\n"\
         "ps, pdf, png and svg files.\n\n");
  }
else
  {
  printf("\n\nFinished.\n\n");
  }

return(1);
}



