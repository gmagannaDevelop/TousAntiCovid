
#include "libraries.h"

void (*directions[])(Coordinate *pos, int, int) = {
    /* This "directions" array will stablish an
       order for the directions, and more importantly,
       it will allow "counter-gradient" movements.
            {NW,  N, NW,  O, SE, S, SW, E}
            {-1, -2, -3, -4, 1,  2,  3, 4} 
            { 0,  1,  2,  3, 4,  5,  6, 7}
       as one can see here, oposite directions 
       have the same value but different sign.
       With this layout we can easily 
       "inverse the direction", i.e. 
       "move against the gradient".
     */
    move_NW, move_N, move_NE, move_W,
    move_SE, move_S, move_SW, move_E
};

int person_death(
    Person *p, struct singly_linked_list **p_persons,
    Case **p_table, int n, int m
){
    Case *table = *p_table;
    struct singly_linked_list *persons = *p_persons;
    Case *current = &table[ p->pos.y*m + p->pos.x ];

    if (NULL == current->p ){
        printf("\tWtf bro, the case is already empty !\n");
        return FALSE;
    } else {
        current->p = NULL;
        return remove_person_from_sll(persons, p);
    }
}

int move_person(
    Person *p, Case **p_table, int n, int m
){
    Coordinate tmp_pos;
    Case *table = *p_table;
    Case *current = &table[ p->pos.y*m + p->pos.x ];
    Case *next;

    tmp_pos = p->pos;
    // Shift tmp_pos in the direction 
    // mandated by the person :
    directions[p->direction](&tmp_pos, n, m);    
    next = &table[ tmp_pos.y*m + tmp_pos.x];
    
    // if case is empty, move the person
    if (NULL == next->p){
        current->p = NULL;
        directions[p->direction](&(p->pos), n, m);
        next->p = p;
        return TRUE;
    } 
    else {
        p->direction = oposite_direction(p);
        move_person(p, p_table, n, m);
    }
    return FALSE;
}