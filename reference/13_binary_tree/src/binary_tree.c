/*
 * The binary tree which is implemented here will 
 * always end with 'end cap' nodes on all of its leaves.
 * An end cap has 'content' = 0 and 'left' = 'right' = NULL.
 * The 0 'content' of an end cap is not counted as
 * a content of the binary tree.
 *
 */
#include "libraries.h"


struct binary_tree *allocate_and_initialize_bintree\
                   (struct binary_tree **bintree)
{
*bintree = (struct binary_tree *)malloc(sizeof(struct binary_tree));
if(NULL == *bintree)
  {
  printf("\n\nAllocation error in function"\
         " 'allocate_and_initialize_bintree()'\n\n");
  exit(0);
  }

(*bintree)->content = 0;  /* This is equivalent to (*(*bintree)).content */
(*bintree)->left  = NULL;
(*bintree)->right = NULL;
return(*bintree);
}



int extend_bintree(struct binary_tree *bintree, int content)
{
/* Reaching a tree leaf and putting the content there: */
if((NULL == bintree->left) && (NULL == bintree->right))
  {
  bintree->left  = (struct binary_tree *)malloc(sizeof(struct binary_tree));
  bintree->right = (struct binary_tree *)malloc(sizeof(struct binary_tree));
  if((NULL == bintree->left) || (NULL == bintree->right))
    {
    printf("\n\nAllocation error in function"\
           " 'extend_bintree()'\n\n");
    exit(0);
    }
  bintree->content = content;

  bintree->left->content = 0;
  bintree->left->left = NULL;
  bintree->left->right = NULL;

  bintree->right->content = 0;
  bintree->right->left = NULL;
  bintree->right->right = NULL;
  return(content);
  }

/* Going to the left branch: */
if(content < bintree->content)
  {
  return(extend_bintree(bintree->left, content));
  }

/* Going to the right branch: */
return(extend_bintree(bintree->right, content));
}



int bintree_size(struct binary_tree *bintree)
{
if((NULL == bintree->left) && (NULL == bintree->right))
  {
  return(0);
  }
return(1 + bintree_size(bintree->left) + bintree_size(bintree->right)); 
}



int bintree_max_entry(struct binary_tree *bintree)
{
/*
 * Returns the largest content stored in the binary tree
 * or 0 is the tree is empty
 *
 */
if((NULL == bintree->left) && (NULL == bintree->right))
  {
  return(0);
  }
if(NULL == bintree->right->right)
  {
  return(bintree->content);
  }
return(bintree_max_entry(bintree->right));
}



int bintree_min_entry(struct binary_tree *bintree)
{
/*
 * Returns the smallest content stored in the binary tree
 * or 0 is the tree is empty
 *
 */
if((NULL == bintree->left) && (NULL == bintree->right))
  {
  return(0);
  }
if(NULL == bintree->left->left)
  {
  return(bintree->content);
  }
return(bintree_min_entry(bintree->left));
}



struct binary_tree *bintree_maxcontent_node(struct binary_tree *bintree)
{
/*
 * Returns a pointer to the node with the largest content
 * in 'bintree', or NULL if 'bintree' is an empty tree,
 * or NULL if 'bintree' is NULL.
 *
 */
if(NULL == bintree)
  {
  return(NULL);
  }
if((NULL == bintree->left) && (NULL == bintree->right))
  {
  return(NULL);
  }
if(NULL == bintree->right->right)
  {
  return(bintree);
  }
return(bintree_maxcontent_node(bintree->right));
}



void empty_bintree(struct binary_tree *bintree)
{
while((NULL != bintree->left) && (NULL != bintree->right))
  {
  remove_node_from_bintree(bintree, bintree->content);
  }
}



int remove_node_from_bintree(struct binary_tree *bintree, int content)
{
/*
 * Removes the first (that is: upmost, closest to the tree trunk)
 * node with content 'content' from a given binary tree.
 * returns 0 if 'content' was not found in the tree,
 * or 1 upon success.
 *
 */
struct binary_tree *node_to_remove;
struct binary_tree *tmp_new_leftbranch;
struct binary_tree *tmp_new_rightbranch;
struct binary_tree *tmp_old_rightbranch;
struct binary_tree *maxend_of_new_node;
int tmp_new_content;

node_to_remove = search_content_in_bintree(bintree, content);
if(NULL == node_to_remove)
  {
  return(0);
  }

tmp_new_leftbranch  = node_to_remove->left->left;
tmp_new_rightbranch = node_to_remove->left->right;
tmp_new_content = node_to_remove->left->content;

tmp_old_rightbranch = node_to_remove->right;

free(node_to_remove->left);

node_to_remove->content = tmp_new_content;
node_to_remove->left  = tmp_new_leftbranch;
node_to_remove->right = tmp_new_rightbranch;

/*
 * Descent to the rightmost end node of tmp_new_rightbranch,
 * and append the 'tmp_old_rightbranch' pointer to it:
 *
 */
maxend_of_new_node = bintree_maxcontent_node(node_to_remove);
if(NULL != maxend_of_new_node)
  {
  free(maxend_of_new_node->right);
  maxend_of_new_node->right = tmp_old_rightbranch;
  }
/* in following 'else' case, 'node_to_remove' is an end cap [0] 
 *                                                          | |    
 *                                                       NULL  NULL   
 */
else
  {
  tmp_new_leftbranch = tmp_old_rightbranch->left;
  tmp_new_rightbranch = tmp_old_rightbranch->right;
  tmp_new_content = tmp_old_rightbranch->content;

  free(tmp_old_rightbranch);

  node_to_remove->content = tmp_new_content;
  node_to_remove->left  = tmp_new_leftbranch;
  node_to_remove->right = tmp_new_rightbranch;  
  }

return(1);
}



struct binary_tree *search_content_in_bintree(struct binary_tree *bintree,\
                                              int content)
{
/* 
 * Returns a pointer to the first (that is: upmost, closest to the tree trunk)
 * node with content 'content' or NULL if content is not present in the tree.
 *
 */
if((NULL == bintree->left) && (NULL == bintree->right))
  {
  return(NULL);
  }

if(bintree->content == content)
  {
  return(bintree);
  }

if(content < bintree->content)
  {
  return(search_content_in_bintree(bintree->left, content));
  }
return(search_content_in_bintree(bintree->right, content));
}



int write_bintree_dotfile(struct binary_tree *bintree, char *filename)
{
/*
 * Writes a file in the dot format, specifying the graph of
 * the binary tree, and a shell script to plot the graph using
 * the dot command.
 *
 * Duplicate entries in the binary tree will be represented by separate
 * vertices in the graph.
 *
 */
int nodecounter = 0;
FILE *outputfile;

if((NULL == bintree->left) && (NULL == bintree->right))
  {
  return(0);
  }

outputfile = fopen(filename, "w");
if(NULL == outputfile)
  {
  printf("\n\nCould not open the file '%s' for writing.\n\n", filename);
  return(0);
  }

fprintf(outputfile, "digraph bintree\n{\noverlap = false\nsplines = true\n");

add_bintree_node_and_branches_to_dot(bintree, outputfile, &nodecounter);

fprintf(outputfile, "}");
fclose(outputfile);

outputfile = fopen("shellscript_dotgraph.sh", "w");
if(NULL == outputfile)
  {
  printf("\n\nCould not open the file 'shellscript_dotgraph.sh' for writing.\n\n");
  return(0);
  }
fprintf(outputfile, "rm bintree_graph.ps;\n"\
                    "rm bintree_graph.pdf;\n"\
                    "rm bintree_graph.svg;\n"\
                    "rm bintree_graph.png;\n"\
                    "dot -Tps %s -o bintree_graph.ps;\n"\
                    "dot -Tpdf %s -o bintree_graph.pdf;\n"\
                    "dot -Tsvg %s -o bintree_graph.svg;\n"\
                    "dot -Tpng %s -o bintree_graph.png;",\
                     filename, filename, filename, filename); 
if(-1 == system("chmod +x shellscript_dotgraph.sh"))
  {
  printf("\n\nCould not make the file 'shellscript_dotgraph.sh' executable.\n\n");
  }

return(1);
}



int add_bintree_node_and_branches_to_dot(struct binary_tree *bintree,\
                                         FILE *outputfile, int *nodecounter)
{
/*
 * Duplicate entries in the binary tree will be represented by separate
 * vertices in the graph.
 *
 * If the first argument of this function is the trunk of a binary tree, 
 * then last argument of this function should be a pointer to an integer
 * that is equal to zero. 
 *
 */
int thisnode = 0;

if((NULL == bintree->left) && (NULL == bintree->right))
  {
  return(0);
  }

(*nodecounter)++;
thisnode = *nodecounter;
fprintf(outputfile, "%d [label = \"%d\"]\n", thisnode, bintree->content);

if((NULL == bintree->left->left) && (NULL == bintree->right->right))
  {
  fprintf(outputfile, "%d\n", thisnode);
  return(1);
  }

if(NULL != bintree->left->left)
  {
  fprintf(outputfile, "%d [label = \"%d\"]\n", (*nodecounter)+1, bintree->left->content);
  fprintf(outputfile, "%d -> %d\n", thisnode, (*nodecounter)+1);
  add_bintree_node_and_branches_to_dot(bintree->left, outputfile, nodecounter);
  }

if(NULL != bintree->right->right)
  {
  fprintf(outputfile, "%d [label = \"%d\"]\n", (*nodecounter)+1, bintree->right->content);
  fprintf(outputfile, "%d -> %d\n", thisnode, (*nodecounter)+1);
  add_bintree_node_and_branches_to_dot(bintree->right, outputfile, nodecounter);
  }

return(1);
}

