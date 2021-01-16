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
    
    // FUNCTION print_sll(struct singly_linked_list *sll);
    /*
     * Print the set of references to 
     * people stored in the list's nodes.
     * These are memory adresses so it is only
     * intended to be used for debugging.
     */
    int print_sll(struct singly_linked_list *sll);
    

    // FUNCTION extend_sll(struct singly_linked_list *sll, Person *p);
    /*
     * Example behaviors for p = 0x00ff:
     *   [NULL]->NULL
     *     becomes 
     *      [0x00f]->[NULL]->NULL
     *  
     *   [0x0001]->[0x0002]->[0x0003]->[NULL]->NULL 
     *      becomes
     *      [0x0001]->[0x0002]->[0x0003]->[0x00ff]->[NULL]->NULL 
     */
    int extend_sll(
        struct singly_linked_list *sll, 
        Person *p
    );
    
    Person *extend_sll_at_head(
        struct singly_linked_list **sll,
        Person *p
    );
    
    // FUNCTION extend_sll_return_lastnode(struct singly_linked_list *sll, Person *p);
    /*
     * Same as 'extend_sll', but also returns a pointer
     * to the last node of the sll before the end cap node.
     *
     */
    struct singly_linked_list *extend_sll_return_lastnode(
        struct singly_linked_list *sll, 
        Person *p
    );
    
    /*  O(n) recursive search */
    int is_in_sll(struct singly_linked_list *sll, Person *p);
    
    // FUNCTION remove_person_from_sll(struct singly_linked_list *sll, Person *p);
     /* Recursively walk the list until the person to be removed is found.
        If the person is in the list, its node won't be removed.
        In order to keep the list integrity, we must:
         1. Fnd the person to be removed
         2. Take the person to be removed
         3. Free the pointer to that person (but not the node)
         4. Re-arrange references :
            4.1 Current node will point to the next node's person
            4.2 Current node next will point to the next node's next
         5. Remove next node.

        Example :

                person1  person2  person3    NULL
        begin -> [ ^ ] -> [ ^ ] -> [ ^ ] -> [ ^ ] -> NULL

        `remove person 2`

                person1           person3    NULL
        begin -> [ ^ ] -> [ ^ ] -> [ ^ ] -> [ ^ ] -> NULL

                person1     ~----->person3   NULL
        begin -> [ ^ ] -> [ ^ ] -v [ ^ ] -> [ ^ ] -> NULL
                                 ~------------^

                person1     ~----->person3   NULL
        begin -> [ ^ ] -> [ ^ ] -v          [ ^ ] -> NULL
                                 ~------------^
    
                person1   person3   NULL
        begin -> [ ^ ] -> [ ^ ] -> [ ^ ] -> NULL
    */
    int remove_person_from_sll(struct singly_linked_list *sll, Person *p);
    
    /* Call pop_last_node_from_sll()
        until the list is empty, freeing the 
        pointers to avoid memory leaks.
    */
    int empty_sll(struct singly_linked_list *sll);
    
    Person *pop_last_node_from_sll(struct singly_linked_list *sll);


#endif
