
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

int max_danger_direction(
    Person *p, Case **p_table, int n, int m
)
{
  Coordinate tmp_pos;
  Case *table = *p_table;
  int i, danger_max, dir_max_danger;
  int dangers[N_DIRECTIONS];

  for (i=0; i<N_DIRECTIONS; i++){
    tmp_pos = p->pos;
    directions[i](&tmp_pos, n, m);
    dangers[i] = table[ tmp_pos.y*m + tmp_pos.x ].danger;
  }

  danger_max = dir_max_danger = 0;
  for (i=0; i<N_DIRECTIONS; i++){
    if (danger_max < dangers[i]){
      danger_max = dangers[i];
      dir_max_danger = i;
    }
  }

  return dir_max_danger;
}


void add_danger(Person *p, Case **p_table, int n, int m){
  Case *table = *p_table;
  int i=-2,j=-2;
  for (i=-2;i<3;i++){
    for (j=-2;j<3;j++){
      if ((i<2) && (i>-2) && (j<2) && (j>-2)){
        table[(p->pos.y+j+n)%n*m + (p->pos.x+i+m)%m].danger += DANGER_CLOSE;
      }else{
        table[ (p->pos.y+j+n)%n*m + (p->pos.x+i+m)%m ].danger += DANGER_FAR;
      }
    }
  }
  table[(p->pos.y)*m + (p->pos.x)].danger-=DANGER_CLOSE;
}

void rm_danger(Person *p, Case **p_table, int n, int m){
  Case *table = *p_table;
  int i=-2,j=-2;
  for (i=-2;i<3;i++){
    for (j=-2;j<3;j++){
      if ((i<2) && (i>-2) && (j<2) && (j>-2)){
        table[(p->pos.y+j+n)%n*m + (p->pos.x+i+m)%m].danger -= DANGER_CLOSE;
      }else{
        table[ (p->pos.y+j+n)%n*m  + (p->pos.x+i+m)%m ].danger -= DANGER_FAR;
      }
    }
  }
  table[(p->pos.y)*m + (p->pos.x)].danger += DANGER_CLOSE;
}

/* TODO : UNIFY INTERFACE (Y,X) */
void init_person_at(Person *p, int x, int y, int d)
{
    p->symptomatic = FALSE;
    p->viral_charge = 0;
    p->pos.y = y;
    p->pos.x = x;
    p->direction = d;
    p->healing = FALSE;
}

void init_doctor_at(Person *p, int x, int y, int d)
{
    p->symptomatic = FALSE;
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

    if (NULL == current->p){
        printf("\tWtf bro, the case is already empty !\n");
        return FALSE;
    } else {
        if (p->symptomatic){ // todo : verify if person is being healed.
          rm_danger(p,p_table,n,m);
        }
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
    // P_MOVE_RANDOM := 1.0 - P_MOVE_MOMENTUM
    // persons may change they direction at random
    if (bernoulli_trial(randgen, P_MOVE_RANDOM)){
      p->direction = draw_randint_0n(randgen, N_DIRECTIONS);
    }
    // Shift tmp_pos in the direction mandated by the person :
    directions[p->direction](&tmp_pos, n, m);
    next = &table[ tmp_pos.y*m + tmp_pos.x];

    // if case is empty and it's less dangerous than the current location
    if ((NULL == next->p) && (next->danger<=current->danger)){
        current->p = NULL;
        directions[p->direction](&(p->pos), n, m);
        next->p = p;
        if ((next->viral_charge > 0) && (p->viral_charge == 0)){
            p->viral_charge = MEAN_INFECTION_LENGTH;
            p->symptomatic = bernoulli_trial(randgen, P_SYMPTOMATIC);
            if (p->symptomatic){
              add_danger(p,p_table,n,m);
            }
        }
    } 
    // if case is empty and more dangerous than the current location
    else if (NULL == next->p){
      p->direction = oposite_direction(p);
      tmp_pos = p->pos;
      directions[p->direction](&tmp_pos, n, m);
      next = &table[ tmp_pos.y*m + tmp_pos.x];
      if ((NULL == next->p) && (next->danger<=current->danger)){
          current->p = NULL;
          directions[p->direction](&(p->pos), n, m);
          next->p = p;
          if ((next->viral_charge > 0) && (p->viral_charge == 0)){
              p->viral_charge = MEAN_INFECTION_LENGTH;
              p->symptomatic = bernoulli_trial(randgen, P_SYMPTOMATIC);
              if (p->symptomatic){
                add_danger(p,p_table,n,m);
              }
          }
      }
      else {
        p->direction = oposite_direction(p);
      } 
    }
    else {
        // make it go back
        tmp_pos = p->pos;
        //p->direction = opposite_direction(p);
        p->direction = draw_randint_0n(randgen, N_DIRECTIONS);
        directions[p->direction](&tmp_pos, n, m);
        next = &table[ tmp_pos.y*m + tmp_pos.x];
        if ((NULL == next->p) && (next->danger<=current->danger)){
            // if the case in the just drawn random direction
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

int move_doctor(
    gsl_rng **randgen, Person *p, Case **p_table, int n, int m
){
    Coordinate tmp_pos;
    Case *table = *p_table;
    Case *current = &table[ p->pos.y*m + p->pos.x ];
    Case *next;

    tmp_pos = p->pos;
    //directions[p->direction](&tmp_pos, n, m);
    next = &table[ tmp_pos.y*m + tmp_pos.x];
    int i=-2,j=-2;
    for (i=-2;i<3;i++){
      for (j=-2;j<3;j++){
          if ((i<2) && (i>-2) && (j<2) && (j>-2)){
            if (table[(tmp_pos.y+j+n)%n*m+(tmp_pos.x+i+m)%m].danger>next->danger){
              next = &table[(tmp_pos.y+j+n)%n*m+(tmp_pos.x+i+m)%m];
            }
          }else{
            if (table[(tmp_pos.y+j+n)%n*m+(tmp_pos.x+i+m)%m].danger>next->danger){
              if (i>0 && j>0){
                next = &table[(tmp_pos.y+j-1+n)%n*m+(tmp_pos.x+i-1+m)%m];
              }else if (i>0 && j<0){
                next = &table[(tmp_pos.y+j-1+n)%n*m+(tmp_pos.x+i+1+m)%m];
              }else if (i<0 && j>0){
                next = &table[(tmp_pos.y+j+1+n)%n*m+(tmp_pos.x+i-1+m)%m];
              }else if (i<0 && j<0){
                next = &table[(tmp_pos.y+j+1+n)%n*m+(tmp_pos.x+i+1+m)%m];
              }
            }
          }
      }
      if (current == next){
        directions[p->direction](&tmp_pos, n, m);
        next = &table[ tmp_pos.y*m + tmp_pos.x];
      }
    }


    // if case is empty, move the person
    if ((NULL == next->p) && (next->danger>=current->danger)){
        current->p = NULL;
        //directions[p->direction](&(p->pos), n, m);
        next->p = p;
        if ((next->viral_charge > 0) && (p->viral_charge == 0)){
            p->viral_charge = MEAN_INFECTION_LENGTH;
            p->symptomatic = bernoulli_trial(randgen, P_SYMPTOMATIC);
            if (p->symptomatic){
              add_danger(p,p_table,n,m);
            }
        }
    }
    else {
        // make it go back
        tmp_pos = p->pos;
        //p->direction = opposite_direction(p);
        //p->direction = draw_randint_0n(randgen, N_DIRECTIONS);
        //directions[p->direction](&tmp_pos, n, m);
        next = &table[ tmp_pos.y*m + tmp_pos.x];
        if ((NULL == next->p) && (next->danger>=current->danger)){
            // if the case in the just drawn random direction
            // is free, move person in this direction.
            //printf("go back \n");
            move_doctor(randgen, p, p_table, n, m);
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
  int i, j;
  Person *p;
  struct singly_linked_list *p_iter;

  p_iter = *people;
  while(p_iter->next != NULL){
    p = p_iter->p;
    // "viral tests"
    if (p->symptomatic){
      if (bernoulli_trial(randgen, VIRULENCE)){
        (*table)[ p->pos.y * M + p->pos.x ].viral_charge = VIRAL_LIFESPAN;
        person_death(p, people, table, N, M);
      }
      else {
          // this condition does not obey the problem statement
          // might need to delete.
          if (p->viral_charge > 0){ p->viral_charge--; }
          else {
            p->symptomatic = FALSE;
            rm_danger(p,table,N,M);
          }
      }
    }
    else {
      if (p->viral_charge > 0){
          p->viral_charge--;
      }
      if (bernoulli_trial(randgen, P_MOVE)){
        move_person(randgen, p, table, N, M);
      }
      p_iter = p_iter->next;
    }
  }

  p_iter = *doctors;
  while(p_iter->next != NULL){
    p = p_iter->p;
    // "viral tests"
    if (p->symptomatic){
      if (bernoulli_trial(randgen, VIRULENCE)){
        (*table)[ p->pos.y * M + p->pos.x ].viral_charge = VIRAL_LIFESPAN;
        person_death(p, doctors, table, N, M);
      } else {
          if (p->viral_charge > 0){ p->viral_charge--; }
          else {
            p->symptomatic = FALSE;
            rm_danger(p,table,N,M);
          }
      }
    }
    else {
      if (p->viral_charge > 0){
          p->viral_charge--;
      }
      if (bernoulli_trial(randgen, P_MOVE)){
        move_person(randgen, p, table, N, M);
      }
      p_iter = p_iter->next;
    }

  }

  for (i = 0; i<N; i++){
    for (j = 0; j<M; j++){
      if ((*table)[ i*M + j].viral_charge > 0){
        (*table)[ i*M + j ].viral_charge--;
      }
    }
  }

  return TRUE;
}
