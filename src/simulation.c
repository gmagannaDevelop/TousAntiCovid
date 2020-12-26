
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

/* TODO : UNIFY INTERFACE (Y,X) */
void init_person_at(Person *p, int x, int y, int d)
{
    p->alive = TRUE;
    p->viral_charge = 0;
    p->pos.y = y;
    p->pos.x = x;
    p->direction = d;
    p->healing = FALSE;
}

void init_doctor_at(Person *p, int x, int y, int d)
{
    p->alive = TRUE;
    p->viral_charge = 0;
    p->pos.x = x;
    p->pos.y = y;
    p->direction = d;
    p->healing = TRUE;
}

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
    gsl_rng **randgen, Person *p, Case **p_table, int n, int m
){
    Coordinate tmp_pos;
    Case *table = *p_table;
    Case *current = &table[ p->pos.y*m + p->pos.x ];
    Case *next;

    tmp_pos = p->pos;
    // Shift tmp_pos in the direction 
    // mandated by the person :
    //printf("person's direction : %d\n", p->direction);
    directions[p->direction](&tmp_pos, n, m);    
    next = &table[ tmp_pos.y*m + tmp_pos.x];
    
    // if case is empty, move the person
    if (NULL == next->p){
        current->p = NULL;
        directions[p->direction](&(p->pos), n, m);
        next->p = p;
    } 
    else {
        // make it go back 
        tmp_pos = p->pos;
        p->direction = oposite_direction(p);
        directions[p->direction](&tmp_pos, n, m);    
        next = &table[ tmp_pos.y*m + tmp_pos.x];
        if (NULL == next->p){
            // if the case in the opposite direction
            // is free, move person in this direction.
            //printf("go back \n");
            move_person(randgen, p, p_table, n, m);
        }
        else {
            return FALSE;
            // try getting a random new direction to 
            // avoid infinite recursion :
            //p->direction = draw_randint_0n(randgen, N_DIRECTIONS);
            //printf("en mode random \n");
            //move_person(randgen, p, p_table, n, m);         
            // turns out that random dirrections 
            // can also lead to infinite recursion.
        } 
    }
    return TRUE;
}


int global_update(
    gsl_rng **randgen,
    struct singly_linked_list **people, 
    struct singly_linked_list **doctors,
    Case **table, int N, int M
){
  Person *p;
  struct singly_linked_list *p_iter;
  
  p_iter = *people;
  while(p_iter->next != NULL){
    //printf("update one person\n");
    p = p_iter->p;
    // test deadly virus :
    if (bernoulli_trial(randgen, 0.01)){
      person_death(p, people, table, N, M);
    }
    else {
      move_person(randgen, p, table, N, M);
      p_iter = p_iter->next;
    }
    //printf("DONE\n");
  }
  p_iter = *doctors;
  while(p_iter->next != NULL){
    //printf("update one doctor\n");  
    p = p_iter->p;
    move_person(randgen, p, table, N, M);
    p_iter = p_iter->next;
    //printf("DONE\n");
  }
  return TRUE;
}


