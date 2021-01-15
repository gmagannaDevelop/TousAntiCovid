/*
 *
 */

#include "libraries.h"


int main(int argc, char **argv)
{
    int N, M, step, save_graphics;
    int row, column; /* Counters */
    char filename[MAX_LINELENGTH];
    struct SDL_graphics *SDL_graphics;
    SDL_Event event;
    FILE *outputscript;
    Case *table, *itable;
    Person *p, *d;
    struct singly_linked_list *people, *doctors, *p_iter;
    double plambda, pdoctor, pvirus;
    double p_init_lambda, p_init_doctor, p_init_virus;
    int nlambda, ndoctor, nvirus;
    gsl_rng *randgen;

    parse_commandline(
      argc, argv, &N, &M,
      &p_init_lambda, &p_init_doctor, &p_init_virus,
      &save_graphics
    );

    //N = 30;
    //M = 80;
    //p_init_lambda = 0.20;
    //p_init_doctor = 0.10;
    //p_init_virus = 0.05;
    //save_graphics = 0;

    correct_posterior_probs(&p_init_lambda, &p_init_doctor, &p_init_virus);

    initialize_randgen(&randgen, RND_VERBOSITY);

    table = (Case *)malloc( N * M * sizeof(Case) );
    if (NULL == table){ printf("matrix allocation error\n"); exit(EXIT_FAILURE); }

    allocate_and_initialize_sll(&people);
    allocate_and_initialize_sll(&doctors);

    nlambda = ndoctor = nvirus = 0;
    for (row=0; row<N; row++){
      for (column=0; column<M; column++){
        // P INIT LAMBDA
        itable = &table[row*M + column];
        if (TRUE == bernoulli_trial(&randgen, p_init_lambda)){
          itable->p = p = (Person *)malloc(sizeof(Person));
          if (NULL == p){ printf("person allocation error\n"); exit(EXIT_FAILURE); }
          extend_sll(people, p);
          // TODO : UNIFY INTERFACE (Y,X)
          init_person_at(p, column, row, draw_randint_0n(&randgen, N_DIRECTIONS));
          nlambda++;
        }
        // P INIT DOCOR
        else if (TRUE == bernoulli_trial(&randgen, p_init_doctor)){
          itable->p = d = (Person *)malloc(sizeof(Person));
          if (NULL == d){ printf("person allocation error\n"); exit(EXIT_FAILURE); }
          extend_sll(doctors, d);
          // TODO : UNIFY INTERFACE (Y,X)
          init_doctor_at(d, column, row, draw_randint_0n(&randgen, N_DIRECTIONS));
          ndoctor++;
        }
        // P INIT VIRUS
        else if (TRUE == bernoulli_trial(&randgen, p_init_virus)) {
          itable->viral_charge = VIRAL_LIFESPAN;
          itable->p = NULL;
          itable->danger = 0;
          nvirus++;
        }
        /* Rest of the time */
        else {
          itable->viral_charge = 0;
          itable->p = NULL;
          itable->danger = 0;
        }
      }
    }


    if (FALSE){
      plambda = (double)sll_list_length(people)/((double)N*M) ;
      pdoctor = (double)sll_list_length(doctors)/((double)N*M) ;
      pvirus = (double)nvirus/((double)N*M) ;
      printf("%f,%f,%f\n", plambda, pdoctor, pvirus);
      //printf("Person count  prior : %d, posterior %d \n", nlambda, sll_list_length(people));
      //printf("Doctor count  prior : %d, posterior %d \n", ndoctor, sll_list_length(doctors));
      //show_grid_lists(table, N, M, people, doctors);
      exit(EXIT_SUCCESS);
    }


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



    outputscript = fopen("ppm_to_gif_script.sh", "w");
    if(NULL == outputscript)
      {
      printf("\n\nCould not open file 'ppm_to_gif_script.sh' for writing.\n\n");
      empty_sll(people);
      empty_sll(doctors);
      free(table);
      exit(0);
      }
    if(-1 == system("chmod +x ppm_to_gif_script.sh"))
      {
      printf("\n\nCould not make 'ppm_to_gif_script.sh' executable.\n\n");
      empty_sll(people);
      empty_sll(doctors);
      free(table);
      exit(0);
      }


    for(step = 0; step < MAX_SIMULATION_STEPS; step++){

      msleep(100);
      //printf("Entering function global_update, iteration : %d\n", step);
      global_update(&randgen, &people, &doctors, &table, N, M);
      // show_grid(table, N, M);

      visualise_danger(SDL_graphics, table, N, M, 5);

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

      /* ppm picture file output and gif conversion script entry: */
      if((0 == step%GIF_STEP) && (save_graphics)) {
        sprintf(filename, "Snapshot_%08d.ppm", step+1);
        write_ppm(SDL_graphics, filename);
        fprintf(outputscript, "(convert %s Snapshot_%08d.gif; rm %s)\n", filename, step+1, filename);
      }

      /* Kill SDL if Strg+c was pressed in the stdin console: */
      signal(SIGINT, exit);
      while( SDL_PollEvent(&event) ){
        if( event.type == SDL_KEYDOWN &&\
           (event.key.keysym.sym == SDLK_c &&\
            event.key.keysym.mod & KMOD_CTRL))
          {
            empty_sll(people);
            empty_sll(doctors);
            free(table);
            printf("\n\nGOT KILLED.\n\nRun './ppm_to_gif_script.sh' to convert ppm output to gif.\n\n");
            fclose(outputscript);
            exit(0);
          }
      }
    }

    printf("\n\nFINISHED.\n\nRun './ppm_to_gif_script.sh' to convert ppm output to gif.\n\n");
    fclose(outputscript);
    empty_sll(people);
    empty_sll(doctors);
    free(table);
    return(1);
}
