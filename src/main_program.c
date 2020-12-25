/*
 *
 */ 

#include "libraries.h"


int main(int argc, char **argv)
{
int N, n, step, save_graphics;
float *persons; /* (x,y) coordiantes of all N persons */
int row, column, i, j; /* Counters */
char filename[MAX_LINELENGTH];
struct SDL_graphics *SDL_graphics; 
SDL_Event event;
FILE *outputscript;
Case *table, *itable, ttable;
Person *p, *d;
struct singly_linked_list *people, *doctors, *p_iter;
double plambda, pdoctor, pvirus;
int nlambda, ndoctor, nvirus;
gsl_rng *randgen;

//parse_commandline(argc, argv, &N, &save_graphics);
N = 10;
save_graphics = 0;

initialize_randgen(&randgen, RND_VERBOSITY);

table = (Case *)malloc( N_LINES * M_COLUMNS * sizeof(Case) );
if (NULL == table){ printf("matrix allocation error\n"); exit(EXIT_FAILURE); }

allocate_and_initialize_sll(&people);
allocate_and_initialize_sll(&doctors);

nlambda = ndoctor = nvirus = 0;
for (row=0; row<N_LINES; row++){
  for (column=0; column<M_COLUMNS; column++){
    // P INIT LAMBDA
    itable = &table[row*M_COLUMNS + column];
    if (TRUE == bernoulli_trial(&randgen, P_INIT_LAMBDA)){
      itable->p = p = (Person *)malloc(sizeof(Person));
      if (NULL == p){ printf("person allocation error\n"); exit(EXIT_FAILURE); }
      extend_sll(people, p);
      // TODO : UNIFY INTERFACE (Y,X)
      init_person_at(p, column, row, draw_randint_0n(&randgen, N_DIRECTIONS));
      nlambda++;
    }
    // P INIT DOCOR 
    else if (TRUE == bernoulli_trial(&randgen, P_INIT_DOCTOR)){
      itable->p = d = (Person *)malloc(sizeof(Person));
      if (NULL == d){ printf("person allocation error\n"); exit(EXIT_FAILURE); }
      extend_sll(doctors, d);
      // TODO : UNIFY INTERFACE (Y,X) 
      init_doctor_at(d, column, row, draw_randint_0n(&randgen, N_DIRECTIONS));
      ndoctor++;
    }
    // P INIT VIRUS 
    else if (TRUE == bernoulli_trial(&randgen, P_INIT_VIRUS)) {
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
  plambda = (double)sll_list_length(people)/((double)N_LINES*M_COLUMNS) ;
  pdoctor = (double)sll_list_length(doctors)/((double)N_LINES*M_COLUMNS) ;
  pvirus = (double)nvirus/((double)N_LINES*M_COLUMNS) ;
  printf("%f,%f,%f\n", plambda, pdoctor, pvirus);
  //printf("Person count  prior : %d, posterior %d \n", nlambda, sll_list_length(people));
  //printf("Doctor count  prior : %d, posterior %d \n", ndoctor, sll_list_length(doctors));
  //show_grid_lists(table, N_LINES, M_COLUMNS, people, doctors);
}


j = 0;
while (TRUE){
  show_grid_lists(table, N_LINES, M_COLUMNS, people, doctors);
  p_iter = people;
  i = 0;
  while(p_iter->next != NULL){
    p = p_iter->p;
    // test deadly virus :
    if (bernoulli_trial(&randgen, 0.01)){
      person_death(p, &people, &table, N_LINES, M_COLUMNS);
    }
    else {
      move_person(&randgen, p, &table, N_LINES, M_COLUMNS);
      p_iter = p_iter->next;
    }
  }
  p_iter = doctors;
  while(p_iter->next != NULL){
    p = p_iter->p;
    move_person(&randgen, p, &table, N_LINES, M_COLUMNS);
    p_iter = p_iter->next;
  }
  printf(CLEAR);
  show_grid_lists(table, N_LINES, M_COLUMNS, people, doctors);
  msleep(100);
}

exit(EXIT_SUCCESS);


/* Allocate and initialize N persons at random (x,y) positions: ................... */
persons = (float *)malloc(2 * N * sizeof(float));
if(NULL == persons)
  {
  printf("\n\nError in allocating 'persons' array.\n\n");
  exit(0);
  }
initialize_rand();
for(n = 0; n < N; n++)
  {
  persons[2*n  ] = GRAPHICS_MARGIN + rand_zero_n(GRAPHICS_WIDTH  - 2*GRAPHICS_MARGIN);
  persons[2*n+1] = GRAPHICS_MARGIN + rand_zero_n(GRAPHICS_HEIGHT - 2*GRAPHICS_MARGIN);
  }
/* End of allocation and initialization of N persons at random (x,y) positions. ... */


/* SDL graphics allocation and initialization: ........................ */
SDL_graphics=(struct SDL_graphics*)malloc(sizeof(struct SDL_graphics));
if(NULL == SDL_graphics)
  {
  printf("\n\nError in allocating struct SDL_graphics *SDL_graphics\n\n");
  exit(0);
  }
SDL_graphics->width = GRAPHICS_WIDTH;
SDL_graphics->height= GRAPHICS_HEIGHT;
allocate_SDL_pixelarray(SDL_graphics);
initialize_SDL_graphics(SDL_graphics);
initialize_pixel_array(SDL_graphics);
/* End of SDL graphics allocation and initialization .................. */



outputscript = fopen("ppm_to_gif_script.sh", "w");
if(NULL == outputscript)
  {
  printf("\n\nCould not open file 'ppm_to_gif_script.sh' for writing.\n\n");
  free(table);
  exit(0);
  }
if(-1 == system("chmod +x ppm_to_gif_script.sh"))
  {
  printf("\n\nCould not make 'ppm_to_gif_script.sh' executable.\n\n");
  free(table);
  exit(0);
  }


for(step = 0; step < MAX_SIMULATION_STEPS; step++){
  /*
  p = (Person *)malloc(sizeof(Person));
  extend_sll(people, p);
  init_person_at(p, column, row, 2);
  */
  update_positions(persons, N);
  if (step == 15){
    printf("Sim. step : %d length(people) = %d\n", step, sll_list_length(people));
  }
  if (FALSE){
    empty_sll(people);
    /* show_grid(table, N_LINES, M_COLUMNS);
    */
    printf("Sim. step : %d length(people) = %d\n", step, sll_list_length(people));
    /* This proves the problem of simply removing
       a person from the persons list.
       We'll prove the utility of the 
       person_death() function.
    */
  }
  if (step == 30){
    /* show_grid(table, N_LINES, M_COLUMNS);
    */
    printf("Sim. step : %d length(people) = %d\n", step, sll_list_length(people));
  } 
  /*
  p = pop_last_node_from_sll(people);
  if (NULL != p){
    printf("person[%d,%d] will die :) \n", p->pos.x, p->pos.y);
  }
  free(p);
  p = NULL;
  */  

  /* SDL visualization: */
  for(n = 0; n < N; n++){
    visualize_person(SDL_graphics, persons, n, BLOBSIZE);
  }

  drawbox(SDL_graphics, 
      GRAPHICS_MARGIN, GRAPHICS_WIDTH-GRAPHICS_MARGIN,
      GRAPHICS_MARGIN, GRAPHICS_HEIGHT-GRAPHICS_MARGIN, 
      GRAPHICS_MARGIN/2, 0
  );
  sdl_update(SDL_graphics);
  fade_pixel_array(SDL_graphics, FADER);

  /* ppm picture file output and gif conversion script entry: */
  if((0 == step%GIF_STEP) && (save_graphics == TRUE)) {
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
        free(table);
        printf("\n\nGOT KILLED.\n\nRun './ppm_to_gif_script.sh' to convert ppm output to gif.\n\n");
        fclose(outputscript);
        exit(0);
      }    
  }
}

printf("\n\nFINISHED.\n\nRun './ppm_to_gif_script.sh' to convert ppm output to gif.\n\n");
fclose(outputscript);
free(table);
return(1);
}

