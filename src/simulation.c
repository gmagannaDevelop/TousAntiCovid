
#include "libraries.h"


int person_death(
    Person *p, struct singly_linked_list **p_persons,
    Case **p_table, int n, int m
){
    int dead = FALSE;
    Case *table = *p_table;
    struct singly_linked_list *persons = *p_persons;
    Case *current = &table[ p->pos.y*n + p->pos.x ];
    if (NULL == current->p ){
        /*printf("\tWtf bro, the case is already empty !\n");
        */
    } else {
        current->p = NULL;
    }
    if (TRUE == remove_person_from_sll(persons, p)){
        /*printf("\tRemoved person %p from persons %p\n", (void *)p, (void *)persons);
        */
        dead = TRUE;
    } else {
        /*printf("\tperson %p not found in persons %p\n", (void *)p, (void *)persons);
        */
    }

    return dead;
}