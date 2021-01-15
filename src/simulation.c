
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

int max_danger_direction(Person *p, Case **p_table, int n, int m)
{
  Coordinate tmp_pos;
  Case *table = *p_table;
  int i, danger_max, dir_max_danger;
  int dangers[N_DIRECTIONS];

  for (i=0; i<N_DIRECTIONS; i++){
    tmp_pos = p->pos;
    directions[i](&tmp_pos, n, m);
    if (NULL == table[ tmp_pos.y*m + tmp_pos.x ].p){ 
      dangers[i] = table[ tmp_pos.y*m + tmp_pos.x ].danger;
    }
    else {
      dangers[i] = DUMMY_REJECT_MAX;
    }
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

int min_danger_direction(Person *p, Case **p_table, int n, int m)
{
  Coordinate tmp_pos;
  Case *table = *p_table;
  int i, danger_min, dir_min_danger;
  int dangers[N_DIRECTIONS];

  for (i=0; i<N_DIRECTIONS; i++){
    tmp_pos = p->pos;
    directions[i](&tmp_pos, n, m);
    if (NULL == table[ tmp_pos.y*m + tmp_pos.x ].p){ 
      dangers[i] = table[ tmp_pos.y*m + tmp_pos.x ].danger;
    }
    else {
      dangers[i] = DUMMY_REJECT_MIN;
    }
  }

  danger_min = DUMMY_REJECT_MIN;
  dir_min_danger = 0;
  for (i=0; i<N_DIRECTIONS; i++){
    if (danger_min > dangers[i]){
      danger_min = dangers[i];
      dir_min_danger = i;
    }
  }

  return dir_min_danger;
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

int move_person(gsl_rng **randgen, Person *p, Case **p_table, int n, int m){
    Coordinate tmp_pos;
    Case *table = *p_table;
    Case *current = &table[ p->pos.y*m + p->pos.x ];
    Case *next;

    // P_MOVE_RANDOM := 1.0 - P_MOVE_MOMENTUM
    // persons may change they direction at random
    if (bernoulli_trial(randgen, P_MOVE_RANDOM)){
      p->direction = draw_randint_0n(randgen, N_DIRECTIONS);
    }
    tmp_pos = p->pos;
    // Shift tmp_pos in the direction mandated by the person :
    directions[p->direction](&tmp_pos, n, m);
    next = &table[ tmp_pos.y*m + tmp_pos.x];

    // If the next case is empty
    if (NULL == next->p){
      // and it's less dangerous than the current
      if (next->danger <= current->danger){
        current->p = NULL;
        next->p = p;
        p->pos = tmp_pos;
        if ((next->viral_charge > 0) && (p->viral_charge == 0)){
          p->viral_charge = MEAN_INFECTION_LENGTH;
          p->symptomatic = bernoulli_trial(randgen, P_SYMPTOMATIC);
          if (p->symptomatic){
            add_danger(p,p_table,n,m);
          }
        }
        return TRUE;
      }
      else {
        p->direction = min_danger_direction(p, p_table, n, m);
        tmp_pos = p->pos;
        // Shift tmp_pos in the direction mandated by the person :
        directions[p->direction](&tmp_pos, n, m);
        next = &table[ tmp_pos.y*m + tmp_pos.x];
        if (NULL == next->p){
            current->p = NULL;
            next->p = p;
            p->pos = tmp_pos;
            if ((next->viral_charge > 0) && (p->viral_charge == 0)){
              p->viral_charge = MEAN_INFECTION_LENGTH;
              p->symptomatic = bernoulli_trial(randgen, P_SYMPTOMATIC);
              if (p->symptomatic){
                add_danger(p,p_table,n,m);
              }
            }
            return TRUE;
        } else {
          return FALSE;
        }
      }
    } else { // If the next case is empty
      if (bernoulli_trial(randgen, P_STAY_ON_COLLISION)){
        return FALSE;
      }
      else {
        p->direction = draw_randint_0n(randgen, N_DIRECTIONS);
        return move_person(randgen, p, p_table, n, m);
      }
    }
}

int move_doctor(gsl_rng **randgen, Person *p, Case **p_table, int n, int m)
{
    Coordinate tmp_pos;
    Case *table = *p_table;
    Case *current = &table[ p->pos.y*m + p->pos.x ];
    Case *next;

    // P_MOVE_RANDOM := 1.0 - P_MOVE_MOMENTUM
    // persons may change they direction at random
    if (bernoulli_trial(randgen, P_MOVE_RANDOM)){
      p->direction = draw_randint_0n(randgen, N_DIRECTIONS);
    }
    tmp_pos = p->pos;
    // Shift tmp_pos in the direction mandated by the person :
    directions[p->direction](&tmp_pos, n, m);
    next = &table[ tmp_pos.y*m + tmp_pos.x];

    // If the next case is empty
    if (NULL == next->p){
      // and it's less dangerous than the current
      if (next->danger >= current->danger){
        current->p = NULL;
        next->p = p;
        p->pos = tmp_pos;
        if ((next->viral_charge > 0) && (p->viral_charge == 0)){
          p->viral_charge = MEAN_INFECTION_LENGTH;
          p->symptomatic = bernoulli_trial(randgen, P_SYMPTOMATIC);
          if (p->symptomatic){
            add_danger(p,p_table,n,m);
          }
        }
        return TRUE;
      }
      else {
        p->direction = max_danger_direction(p, p_table, n, m);
        tmp_pos = p->pos;
        // Shift tmp_pos in the direction mandated by the person :
        directions[p->direction](&tmp_pos, n, m);
        next = &table[ tmp_pos.y*m + tmp_pos.x];
        if (NULL == next->p){
            current->p = NULL;
            next->p = p;
            p->pos = tmp_pos;
            if ((next->viral_charge > 0) && (p->viral_charge == 0)){
              p->viral_charge = MEAN_INFECTION_LENGTH;
              p->symptomatic = bernoulli_trial(randgen, P_SYMPTOMATIC);
              if (p->symptomatic){
                add_danger(p,p_table,n,m);
              }
            }
            return TRUE;
        } else {
          return FALSE;
        }
      }
    } else { // If the next case is empty
      if (bernoulli_trial(randgen, P_STAY_ON_COLLISION)){
        return FALSE;
      }
      else {
        p->direction = draw_randint_0n(randgen, N_DIRECTIONS);
        return move_doctor(randgen, p, p_table, n, m);
      }
    }
}



int global_update(
    gsl_rng **randgen,
    struct singly_linked_list **people,
    struct singly_linked_list **doctors,
    Case **table, int N, int M
){
  int i, j, died, sneeze_direction;
  Person *p;
  Coordinate tmp_pos;
  Case *tmp_case;
  struct singly_linked_list *p_iter;

  p_iter = *people;
  while(p_iter->next != NULL){
    died = 0;
    p = p_iter->p;
    // "viral tests"
    if (p->symptomatic){
      if (bernoulli_trial(randgen, VIRULENCE)){
        //printf("Oh no, Juanito died !\n");
        (*table)[ p->pos.y * M + p->pos.x ].viral_charge = VIRAL_LIFESPAN;
        person_death(p, people, table, N, M);
        died = 1;
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
        // contamination :
        sneeze_direction = draw_randint_0n(randgen, 9);
        if (8 == sneeze_direction){
          (*table)[ p->pos.y * M + p->pos.x ].viral_charge = VIRAL_LIFESPAN;
        } 
        else {
          tmp_pos = p->pos;
          directions[sneeze_direction](&tmp_pos, N, M);
          tmp_case = &((*table)[ tmp_pos.y * M + tmp_pos.x ]); 
          tmp_case->viral_charge = VIRAL_LIFESPAN;
          if (NULL != tmp_case->p){
            if ((FALSE == tmp_case->p->healing) &&\
                (0 == tmp_case->p->viral_charge))
            {
              tmp_case->p->viral_charge = MEAN_INFECTION_LENGTH;
              if (bernoulli_trial(randgen, P_SYMPTOMATIC)){
                tmp_case->p->symptomatic = TRUE;
                add_danger(p, table, N, M);
              }
            } 
          }
        }
        p->viral_charge--;
      }
      
      if (bernoulli_trial(randgen, P_MOVE)){
        move_person(randgen, p, table, N, M);
      }
    }
    if (0 == died){
      p_iter = p_iter->next;
    }
  }

  p_iter = *doctors;
  while(p_iter->next != NULL){
    died = 0;
    p = p_iter->p;
    // "viral tests"
    if (p->symptomatic){
      if (bernoulli_trial(randgen, VIRULENCE)){
        (*table)[ p->pos.y * M + p->pos.x ].viral_charge = VIRAL_LIFESPAN;
        person_death(p, doctors, table, N, M);
        died = 1;
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
        move_doctor(randgen, p, table, N, M);
      }
    }
    if(0 == died){
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
