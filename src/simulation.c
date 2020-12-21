
#include "libraries.h"


int person_death(
    Person *p, struct singly_linked_list *persons,
    Case *table, int n, int m
){
    int dead = FALSE;
    Case *current = &table[ n*p->pos.y + p->pos.x ];
    if (NULL == current->p){
        printf("Wtf bro, the case is already empty !\n");
    } else {
        current->p = NULL;
    }
    if (TRUE == remove_person_from_sll(persons, p)){
        printf("Removed person %p from persons %p", (void *)p, (void *)persons);
        dead = TRUE;
    } else {
        printf("person %p not found in persons %p", (void *)p, (void *)persons);
    }

    return dead;
}