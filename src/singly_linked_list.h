#ifndef __yt7854_SINGLYLINKEDLIST_H
#define __yt7854_SINGLYLINKEDLIST_H

    #include "datastructures.h"

    struct singly_linked_list{
        Person *p;
        struct singly_linked_list *next;
    };
    
    
    struct singly_linked_list *allocate_and_initialize_sll\
                          (struct singly_linked_list **sll);
    
    int sll_list_length(struct singly_linked_list *sll);
    
    int print_sll(struct singly_linked_list *sll);
    
    int extend_sll(
        struct singly_linked_list *sll, 
        Person *p
    );
    
    Person *extend_sll_at_head(
        struct singly_linked_list **sll,
        Person *p
    );
    
    struct singly_linked_list *extend_sll_return_lastnode(
        struct singly_linked_list *sll, 
        Person *p
    );
    
    int is_in_sll(struct singly_linked_list *sll, Person *p);
    
    int remove_person_from_sll(struct singly_linked_list *sll, Person *p);
    
    int empty_sll(struct singly_linked_list *sll);
    
    Person *remove_last_node_from_sll(struct singly_linked_list *sll);


#endif
