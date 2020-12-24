
#include "libraries.h"


int person_death(
    Person *p, struct singly_linked_list **p_persons,
    Case **p_table, int n, int m
){
    Case *table = *p_table;
    struct singly_linked_list *persons = *p_persons;
    Case *current = &table[ p->pos.y*m + p->pos.x ];

    if (NULL == current->p ){
        printf("\tWtf bro, the case is already empty !\n");
    } else {
        current->p = NULL;
    }

    return remove_person_from_sll(persons, p);
}