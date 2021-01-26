/*
 *
 */

#include "libraries.h"


int main(int argc, char **argv)
{
    int N, M, step, max_sim_steps;
    int row, column; /* Counters */
    int population_size, enable_graphics;
    struct SDL_graphics *SDL_graphics;
    SDL_Event event;
    Case *table, *itable;
    Person *p, *d;
    struct singly_linked_list *people, *doctors, *p_iter;
    double p_init_lambda, p_init_doctor, p_init_virus;
    gsl_rng *randgen;
    Epoch epoch;

    parse_commandline(
      argc, argv, &N, &M,
      &p_init_lambda, &p_init_doctor, &p_init_virus,
      &max_sim_steps, &enable_graphics
    );

    // To enable debugging via gdb https://www.gnu.org/software/gdb/
    // comment out the previous call to parse_commandline()
    // and uncomment the following lines :
    //N = 30;
    //M = 80;
    //p_init_lambda = 0.20;
    //p_init_doctor = 0.10;
    //p_init_virus = 0.05;
    //max_sim_steps = 10000;

    // This function is used to force the empirical density of each agent of 
    // the simulation to be near to the expected one.
    // Writing different conditional statements would have prevented the
    // need for this, but this approach is more flexible as it allows
    // for arbitrary frequencies that are otherwise impossible to obtain.
    // See this issue : https://github.com/gmagannaDevelop/TousAntiCovid/issues/2
    correct_posterior_probs(&p_init_lambda, &p_init_doctor, &p_init_virus);

    initialize_randgen(&randgen, RND_VERBOSITY);

    table = (Case *)malloc( N * M * sizeof(Case) );
    if (NULL == table){ printf("matrix allocation error\n"); exit(EXIT_FAILURE); }

    allocate_and_initialize_sll(&people);
    allocate_and_initialize_sll(&doctors);

    for (row=0; row<N; row++){
      for (column=0; column<M; column++){
        // P INIT LAMBDA
        itable = &table[row*M + column];
        if (TRUE == bernoulli_trial(&randgen, p_init_lambda)){
          itable->p = p = (Person *)malloc(sizeof(Person));
          if (NULL == p){ printf("person allocation error\n"); exit(EXIT_FAILURE); }
          extend_sll(people, p);
          // people are created with random directions :
          //init_person_at(p, column, row, draw_randint_0n(&randgen, N_DIRECTIONS));
          init_person_at(p, column, row, 3);
        }
        // P INIT DOCOR
        else if (TRUE == bernoulli_trial(&randgen, p_init_doctor)){
          itable->p = d = (Person *)malloc(sizeof(Person));
          if (NULL == d){ printf("person allocation error\n"); exit(EXIT_FAILURE); }
          extend_sll(doctors, d);
          // idem. for doctors :
          //init_doctor_at(d, column, row, draw_randint_0n(&randgen, N_DIRECTIONS));
          init_doctor_at(d, column, row, 4);
        }
        // P INIT VIRUS
        else if (TRUE == bernoulli_trial(&randgen, p_init_virus)) {
          itable->viral_charge = VIRAL_LIFESPAN;
          itable->p = NULL;
          itable->danger = 0;
        }
        /* Rest of the time */
        else {
          itable->viral_charge = 0;
          itable->p = NULL;
          itable->danger = 0;
        }
      }
    }

    if (TRUE == enable_graphics){
      /* SDL graphics allocation and initialization: ........................ */
      SDL_graphics=(struct SDL_graphics*)malloc(sizeof(struct SDL_graphics));
      if(NULL == SDL_graphics)
        {
        printf("\n\nError in allocating struct SDL_graphics *SDL_graphics\n\n");
        exit(0);
        }
      SDL_graphics->width = M*SIM_TO_GRAPHICS + 2*GRAPHICS_MARGIN;
      SDL_graphics->height= N*SIM_TO_GRAPHICS + 2*GRAPHICS_MARGIN;
      allocate_SDL_pixelarray(SDL_graphics);
      initialize_SDL_graphics(SDL_graphics);
      initialize_pixel_array(SDL_graphics);
      /* End of SDL graphics allocation and initialization .................. */
    }


    population_size = sll_list_length(people) + sll_list_length(doctors);
    
    epoch.daily_population_size = population_size;
    epoch.grid_viral_charge = 1;
    epoch.new_infections = 0;
    
    step = 0;
    while( (step < max_sim_steps) &&\
           (epoch.daily_population_size > 0) &&\
           (epoch.grid_viral_charge > 0)
    ){
      
    if (TRUE == enable_graphics){
      // signal handling (sub-optimal)
      if( SDL_PollEvent(&event) ){
        if ( event.type == SDL_KEYDOWN &&\
            (event.key.keysym.sym == SDLK_c &&\
            event.key.keysym.mod & KMOD_CTRL))
        {
          printf("EXIT !\n");
          break; 
        }
      }
    }

      // to make the simulation "slower" uncomment and adjust
      // the sleep time in miliseconds :
      msleep(100);
      global_update(&randgen, &people, &doctors, &table, N, M, &epoch);
      
      // to enable (basic) command-line visualisation
      // show_grid(table, N, M);

      // To visualise the "danger gradient" uncomment the following line :
      //visualise_danger(SDL_graphics, table, N, M, 5);

    if (TRUE == enable_graphics){
      visualise_virus(SDL_graphics, table, N, M, VIRUSSIZE);

      // SDL visualization:
      p_iter = people;
      while(p_iter->next != NULL){
        p = p_iter->p;
        visualise_person(SDL_graphics, p, PERSONSIZE);
        p_iter = p_iter->next;
      }

      // SDL visualization:
      p_iter = doctors;
      while(p_iter->next != NULL){
        p = p_iter->p;
        visualise_person(SDL_graphics, p, PERSONSIZE);
        p_iter = p_iter->next;
      }

      drawbox(SDL_graphics,
          GRAPHICS_MARGIN, M*SIM_TO_GRAPHICS + GRAPHICS_MARGIN,
          GRAPHICS_MARGIN, N*SIM_TO_GRAPHICS + GRAPHICS_MARGIN,
          GRAPHICS_MARGIN/2, 0
      );
      sdl_update(SDL_graphics);
      fade_pixel_array(SDL_graphics, FADER);
    }


      // Kill SDL if Strg+c was pressed in the stdin console: 
      // our only effective signal handling 
      signal(SIGINT, exit);
      step++;
    }

    printf("\n\nFINISHED.\n\n");
    empty_sll(people);
    empty_sll(doctors);
    free(table);
    return(1);
}
