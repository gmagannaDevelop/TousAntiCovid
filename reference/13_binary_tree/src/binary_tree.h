#ifndef __y476_BINARYTREE_H
#define __y476_BINARYTREE_H

struct binary_tree
{
int content;
struct binary_tree *left;
struct binary_tree *right;
};


struct binary_tree *allocate_and_initialize_bintree\
                   (struct binary_tree **bintree);

int extend_bintree(struct binary_tree *bintree, int content);

int bintree_size(struct binary_tree *bintree);

int bintree_max_entry(struct binary_tree *bintree);

int bintree_min_entry(struct binary_tree *bintree);

struct binary_tree *bintree_maxcontent_node(struct binary_tree *bintree);

void empty_bintree(struct binary_tree *bintree);

int remove_node_from_bintree(struct binary_tree *bintree, int content);

struct binary_tree *search_content_in_bintree(struct binary_tree *bintree,\
                                              int content);

int write_bintree_dotfile(struct binary_tree *bintree, char *filename);

int add_bintree_node_and_branches_to_dot(struct binary_tree *bintree,\
                                         FILE *outputfile, int *nodecounter);

#endif
