
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

void infection (gsl_rng **randgen, Person *p, Case **p_table, Case *next, int n, int m){
  // This function takes a given person and case.
  Case *table = *p_table;
  // If the person isn't already infected and the target case hosts a virus
  if ((next->viral_charge > 0) && (p->viral_charge == 0)){
    // Person is infected
    p->viral_charge = MEAN_INFECTION_LENGTH;
    // Person might be symptomatic with probability P_SYMPTOMATIC
    p->symptomatic = bernoulli_trial(randgen, P_SYMPTOMATIC);
    // If person is indeed symptomatic
    if (p->symptomatic){
      // Adding danger gradient
      add_danger(p,&table,n,m);
      // If person is a doctor
      if (p->healing){
        // He loses his healing capabilities
        p->healing = FALSE;
      }
    }
  }
}

int find_and_link_patient(Person *d, Case **p_table, int n, int m)
{
  // This function takes a given person (which should be a doctor)
  // And finds a nearby symptomatic which isn't already
  // linked to a doctor and links them both
  // and initiates the healing process
  Coordinate tmp_pos;
  Case *table = *p_table;
  Case *tmp_case;
  int i;

  // Checking all the case in a 1 case range
  for (i=0; i<N_DIRECTIONS; i++){
    tmp_pos = d->pos;
    directions[i](&tmp_pos, n, m);
    tmp_case = &(table[ tmp_pos.y*m + tmp_pos.x ]);
    // If the checked case isn't empty AND
    // contains a symptomatic AND
    // the symptomatic isn't already being healed
    if ((NULL != tmp_case->p) &&\
        (TRUE == tmp_case->p->symptomatic) &&\
        (0 == tmp_case->p->being_healed))
    {
      // Linking the patient to its doctor
      d->p = tmp_case->p;
      // Linking the doctor to its patient
      tmp_case->p->p = d;
      // Initializing the healing process
      tmp_case->p->being_healed = 2;
      // Removing the danger gradient to avoid attracting
      // other doctors
      rm_danger(tmp_case->p, p_table, n, m);
      // Terminates the for-loop as soon as a suitable
      // candidate is found
      return TRUE;
    }
  }
  printf("Doctor found no Juanitos !!!\n");
  // Program termination because it should NEVER happen
  exit(EXIT_FAILURE);
}

int max_danger_direction(Person *p, Case **p_table, int n, int m)
{
  // This function takes a given person and searches for
  // the unoccupied case in a 1-case range with the highest
  // danger gradient
  Coordinate tmp_pos;
  Case *table = *p_table;
  int i, danger_max, dir_max_danger;
  int dangers[N_DIRECTIONS];

  // Checking all the cases in a 1-case range
  for (i=0; i<N_DIRECTIONS; i++){
    tmp_pos = p->pos;
    directions[i](&tmp_pos, n, m);
    // If the checked case is empty
    if (NULL == table[ tmp_pos.y*m + tmp_pos.x ].p){
      // Then store its danger level in a table
      dangers[i] = table[ tmp_pos.y*m + tmp_pos.x ].danger;
    }
    else {
      // If the checked case isn't empty, it'll be
      // attributed a dummy value (here -10)
      // so that there'll always be a better option
      dangers[i] = DUMMY_REJECT_MAX;
    }
  }

  danger_max = dir_max_danger = 0;
  // Searching for the case with the highest danger
  // Which will also be empty
  for (i=0; i<N_DIRECTIONS; i++){
    if (danger_max < dangers[i]){
      danger_max = dangers[i];
      dir_max_danger = i;
    }
  }
  // If none of the case are empty, will return
  // the direction 0. It doesn't really matter since
  // it'll be rechecked before movement and if the case
  // isn't empty, movement will be prevented
  return dir_max_danger;
}

int min_danger_direction(Person *p, Case **p_table, int n, int m)
{
  // Same function as above but searching for the minimum danger
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
  // This function takes a given person and adds a danger
  // gradient around her.
  Case *table = *p_table;
  int i=-2,j=-2;
  // Checking all the cases in a 2-case range
  for (i=-2;i<3;i++){
    for (j=-2;j<3;j++){
      // If the cases are in a 1-case range
      if ((i<2) && (i>-2) && (j<2) && (j>-2)){
        // Then, danger level should be equal to DANGER_CLOSE
        table[(p->pos.y+j+n)%n*m + (p->pos.x+i+m)%m].danger += DANGER_CLOSE;
      }else{
        // If not, it's DANGER_FAR
        table[ (p->pos.y+j+n)%n*m + (p->pos.x+i+m)%m ].danger += DANGER_FAR;
      }
    }
  }
  // Since the case where the person stands is in a 1-case range
  // a danger level = DANGER_CLOSE will be added
  // We don't want that so we substract it
  table[(p->pos.y)*m + (p->pos.x)].danger-=DANGER_CLOSE;
}

void rm_danger(Person *p, Case **p_table, int n, int m){
  // Same function as above but it removes the danger gradient
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
    // Takes a given person and the linked-list it belongs to
    Case *table = *p_table;
    struct singly_linked_list *persons = *p_persons;
    Case *current = &table[ p->pos.y*m + p->pos.x ];

    // Checks if the person really exists (should NEVER happen)
    if (NULL == current->p){
        printf("\tWtf bro, the case is already empty !\n");
        return FALSE;
    } else {
        // if person is being healed, it has a reference to
        // its doctor, that is p->p
        if ((NULL != p->p) && (p->symptomatic)){
          // person->doctor->person
          p->p->p = NULL;
        }
        // If person is symptomatic but isn't being healed
        else if (p->symptomatic){ // todo : verify if person is being healed.
          // Then we didn't remmove the gradient
          // when we began the healing process in
          // find_and_link_patient(), so we need to remove it
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
        infection(randgen, p, p_table, next, n, m);
        return TRUE;
      }
      else {
        // Else, chosing the direction with minimum danger
        p->direction = min_danger_direction(p, p_table, n, m);
        // this direction should be at a empty case
        tmp_pos = p->pos;
        // Shift tmp_pos in the direction mandated by the person :
        directions[p->direction](&tmp_pos, n, m);
        next = &table[ tmp_pos.y*m + tmp_pos.x];
        // We still check if the case is empty
        if (NULL == next->p){
            // Actual movement
            current->p = NULL;
            next->p = p;
            p->pos = tmp_pos;
            // Target case might host a virus, that's why we call infection
            infection(randgen, p, p_table, next, n, m);
            return TRUE;
        } else {
          // Else, if the case wasn't empty
          // It means that all the cases around
          // the person are occupied, then the
          // person doesn't move
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
    // Almost the same function as above BUT:
    // Moves where gradient is the HIGHEST
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
        infection(randgen, p, p_table, next, n, m);
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
            infection(randgen, p, p_table, next, n, m);
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
  // Takes given people and doctors linked list
  int i, j, died, sneeze_direction;
  Person *p;
  Coordinate tmp_pos;
  Case *tmp_case;
  struct singly_linked_list *p_iter;

  // First, the people's (lambda) turn
  p_iter = *people;
  // While there's still people in the linked-list
  while(p_iter->next != NULL){
    died = 0; // The person isn't dead
    p = p_iter->p;
    // If the person is symptomatic
    if (p->symptomatic){
      // And if it is being healed
      if (p->being_healed > 0){
        // Then we decrease the healing countdown
        p->being_healed--;
        // If the person is healed as a result
        if (p->being_healed == 0){
          // Then we update its attributes
          p->viral_charge = 0;
          p->symptomatic = FALSE;
          // Here we remove the links between doctor and patient
          p->p->p = NULL;
          p->p = NULL;
        }
        // Else, if the person is being healed
        // but not cured, person might still die
        else if (bernoulli_trial(randgen, VIRULENCE)){
          //printf("Oh no, Juanito died !\n");
          (*table)[ p->pos.y * M + p->pos.x ].viral_charge = VIRAL_LIFESPAN;
          person_death(p, people, table, N, M);
          died = 1; // Person died
        }
      }
      // Else, if person isn't being healed
      // Person might die
      else if (bernoulli_trial(randgen, VIRULENCE)){
        //printf("Oh no, Juanito died !\n");
        (*table)[ p->pos.y * M + p->pos.x ].viral_charge = VIRAL_LIFESPAN;
        person_death(p, people, table, N, M);
        died = 1; // Person indeed died
      }
      else {
          // Else if symptomatic didn't die, maybe it deserves
          // a chance to survive
          // this condition does not obey the problem statement
          // might need to delete.
          if (p->viral_charge > 1){ p->viral_charge--; }
          // Decrease the viral charge
          else { // Else it means that it somehow survived long enough
            // and is cured
            p->viral_charge--;
            p->symptomatic = FALSE;
            // If the person wasn't being healed
            // Then we need to remove the danger gradient
            // Because it wasn't removed beforehand
            // Might not be necessary but better safe than sorry
            if (0 == p->being_healed){
              rm_danger(p,table,N,M);
            }
          }
      }
    }
    else { // Person isn't symptomatic
      // If the person in infected
      if (p->viral_charge > 0){
        // Then she might infect others, and cases
        // by sneezing on them!
        if (bernoulli_trial(randgen, P_SNEEZE)){
          sneeze_direction = draw_randint_0n(randgen, 9);
          // If person sneezes on the case she stands on
          if (8 == sneeze_direction){
            // Case is contaminated
            (*table)[ p->pos.y * M + p->pos.x ].viral_charge = VIRAL_LIFESPAN;
          }
          else { // Else, it's on of the 7 other possibilities
            tmp_pos = p->pos;
            directions[sneeze_direction](&tmp_pos, N, M);
            tmp_case = &((*table)[ tmp_pos.y * M + tmp_pos.x ]);
            // The case gets updated with a fresh virus
            tmp_case->viral_charge = VIRAL_LIFESPAN;
            // If the case isn't empty
            if (NULL != tmp_case->p){
              // and the person occupying the case isn't a doctor
              if (FALSE == tmp_case->p->healing){
                // then the person gets infected (100% chances)
                infection (randgen, tmp_case->p, table, tmp_case, N, M);
              }
            }
          }
        }
        // Decreasing the viral charge for all asymptomatic
        p->viral_charge--;
      }

      // For all the people (not the symptomatic)
      // Chance of moving!
      if (bernoulli_trial(randgen, P_MOVE)){
        // Might move, that's why move_person is called
        move_person(randgen, p, table, N, M);
      }
    }
    // If the person didn't die
    if (0 == died){
      // Then we switch to the other
      // (The switch is handled elsewhere if the person dies)
      p_iter = p_iter->next;
    }
  }

  // Now it's the doctors' turn!
  // It's globally the same as before
  // BUT doctors don't sneeze, they were MASKS
  // AND they might find patients to heal
  p_iter = *doctors;
  while(p_iter->next != NULL){
    died = 0;
    p = p_iter->p;
    if (p->symptomatic){
      if (p->being_healed > 0){
        p->being_healed--;
        if (p->being_healed == 0){
          p->viral_charge = 0;
          p->healing = TRUE;
          p->symptomatic = FALSE;
          p->p->p = NULL;
          p->p = NULL;
        }
        else if (bernoulli_trial(randgen, VIRULENCE)){
          //printf("Oh no, Juanito died !\n");
          (*table)[ p->pos.y * M + p->pos.x ].viral_charge = VIRAL_LIFESPAN;
          person_death(p, doctors, table, N, M);
          died = 1;
        }
      }
      else if (bernoulli_trial(randgen, VIRULENCE)){
        (*table)[ p->pos.y * M + p->pos.x ].viral_charge = VIRAL_LIFESPAN;
        person_death(p, doctors, table, N, M);
        died = 1;
      } else {
          if (p->viral_charge > 1){
            p->viral_charge--;
          }
          else {
            p->viral_charge--;
            p->symptomatic = FALSE;
            p->healing = TRUE;
            if (0 == p->being_healed){
              rm_danger(p,table,N,M);
            }
          }
      }
    }
    else {
      if (p->viral_charge > 0){
          p->viral_charge--;
      }
      tmp_case = &((*table)[ p->pos.y * M + p->pos.x ]);
      if (NULL == p->p){
        // If the case where the doctor stands
        // Has a gradient equal or higher to
        // DANGER_CLOSE, it means that a symptomatic
        // is close and needs to be healed
        if (DANGER_CLOSE <= tmp_case->danger){
          // Let's find the patient and begin the healing
          find_and_link_patient(p, table, N, M);
        }
        else if (bernoulli_trial(randgen, P_MOVE)){
          move_doctor(randgen, p, table, N, M);
        }
      }
    }
    if(0 == died){
      p_iter = p_iter->next;
    }
  }

  // Checking all the cases of the matrix
  // To decrease the viral charge of the virus
  for (i = 0; i<N; i++){
    for (j = 0; j<M; j++){
      if ((*table)[ i*M + j].viral_charge > 0){
        (*table)[ i*M + j ].viral_charge--;
      }
    }
  }

  return TRUE;
}
