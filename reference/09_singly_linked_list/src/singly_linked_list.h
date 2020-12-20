#ifndef __yt7854_SINGLYLINKEDLIST_H
#define __yt7854_SINGLYLINKEDLIST_H


struct singly_linked_list
{
int id;
struct singly_linked_list *next;
};


struct singly_linked_list *allocate_and_initialize_sll\
                      (struct singly_linked_list **sll);

int sll_list_length(struct singly_linked_list *sll);

int print_sll(struct singly_linked_list *sll);

int extend_sll(struct singly_linked_list *sll, int id);

int extend_sll_at_head(struct singly_linked_list **sll, int id);

struct singly_linked_list *extend_sll_return_lastnode\
         (struct singly_linked_list *sll, int id);

int is_in_sll(struct singly_linked_list *sll, int id);

int remove_id_from_sll(struct singly_linked_list *sll, int id);

int empty_sll(struct singly_linked_list *sll);

int remove_last_node_from_sll(struct singly_linked_list *sll);


#endif
