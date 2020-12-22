/*
 * This program that runs a little social dynamics simulation
 * of N persons in an 'unrequited love' situation:
 *
 * Person 1 is in love with person 2 and walks towards him/her,
 * person 2 is in love with person 3 and walks towards him/her,
 * person 3 is in love with person 4 and walks towards him/her,
 * .
 * .
 * .
 * person N-1 is in love with person N and walks towards him/her,
 * person N is in love with person 1 and walks towards him/her.
 *
 * Graphical output uses two methods:
 * ----------------------------------
 * First, there is an on-the-fly graphics output using the free
 * Simple DirectMedia Layer (SDL) library
 * http://www.libsdl.org/
 * 
 * Second, the program writes uncompressed ppm pictures and
 * converts them on-the-fly into compressed gif pictures via
 * a system call. The gif pictures can later be arranged to
 * give a movie, for example with the OpenShot editor
 * http://www.openshot.org/
 *
 * 
 * M. Heinen, October 2017
 * marco@marco-heinen.de
 *
 */ 

#include "libraries.h"


int main(int argc, char **argv)
{
  #ifdef DEBUGGING
    mtrace();
  #endif
int N, n, step, save_graphics;
float *persons; /* (x,y) coordiantes of all N persons */
int row, column, i, j, tloc, ploc, dloc; /* Counters */
int cpersons, cdoctors, cvirus;
char filename[MAX_LINELENGTH];
struct SDL_graphics *SDL_graphics; 
SDL_Event event;
FILE *outputscript;
Case *table, *itable, ttable;
Person *people, *doctors;
/* struct singly_linked_list *people, *doctors, *p_iter;
*/
double plambda, pdoctor, pvirus;
gsl_rng *randgen;

/* parse_commandline(argc, argv, &N, &save_graphics);
*/

N = 10;
save_graphics = 0;

initialize_randgen(&randgen, RND_VERBOSITY);

/* printf(" N = %d; M = %d; \n", N_LINES, M_COLUMNS);
*/
table = (Case *)malloc( N_LINES * M_COLUMNS * sizeof(Case) );
if (NULL == table){
  printf("Could not allocate [%d,%d]-table of Cases \n",N_LINES, M_COLUMNS);
  exit(EXIT_FAILURE);
}

/* printf("Initialise Cases [%d,%d]\n", N_LINES, M_COLUMNS);
   printf("sizeof(Person) = %ld \n", sizeof(Person)*CHAR_BIT);
   printf("M[row, column], itable, itable->p, p\n");
*/

printf("Initialise table ...\n");
cpersons = 0;
cdoctors = 0;
cvirus = 0;
for (row=0; row<N_LINES; row++){
  for (column=0; column<M_COLUMNS; column++){
    /* P INIT LAMBDA */
    itable = &table[row*N_LINES + column];
    itable->danger = 0;
    itable->viral_charge = 0;
    itable->p = 0;
    itable->d = 0;
    if (TRUE == bernoulli_trial(&randgen, P_INIT_LAMBDA)){
      cpersons++;
      if (!itable->p){
        itable->p = cpersons;
        printf("Marking person [%d] at [%d,%d]\n", cpersons, row, column);
      } else {
        printf("Trying to overwrite a person at [%d,%d] !!!\n\n", row, column);
        exit(EXIT_FAILURE);
      }
    }
    /* P INIT DOCOR */
    else if (TRUE == bernoulli_trial(&randgen, P_INIT_DOCTOR)){
      cdoctors++;
      if (!itable->d){
        itable->d = cdoctors;
        /* printf("Marking doctor at [%d,%d]\n", row, column);
        */
      } else {
        printf("Trying to overwrite a doctor at [%d,%d] !!!\n\n", row, column);
        exit(EXIT_FAILURE);
      }
    }
    /* P INIT VIRUS */
    else if (TRUE == bernoulli_trial(&randgen, P_INIT_VIRUS)) {
      cvirus++;
      itable->viral_charge += 4;
    } 
  }
}

people = (Person *)malloc( cpersons * sizeof(Person) );
if (NULL == people){
  printf("Could not allocate [%d]-array of Persons \n\n",cpersons);
  exit(EXIT_FAILURE);
}
doctors = (Person *)malloc( cdoctors * sizeof(Person) );
if (NULL == doctors){
  printf("Could not allocate [%d]-array of Doctors \n\n",cdoctors);
  exit(EXIT_FAILURE);
}

/* Initialise all persons and doctors */
for (row=0; row<N_LINES; row++){
  for (column=0; column<M_COLUMNS; column++){
    itable = &table[row*N_LINES + column];
    if (0 != itable->p){
      people[itable->p - 1].alive = TRUE;
      people[itable->p - 1].direction = 3;
      people[itable->p - 1].pos.x = column;
      people[itable->p - 1].pos.y = row;
      people[itable->p - 1].viral_charge = 0;
      people[itable->p - 1].healing = FALSE;
    }
    if (0 != itable->d){
      doctors[itable->p - 1].alive = TRUE;
      doctors[itable->p - 1].direction = 3;
      doctors[itable->p - 1].pos.x = column;
      doctors[itable->p - 1].pos.y = row;
      doctors[itable->p - 1].viral_charge = 0;
      doctors[itable->p - 1].healing = TRUE;
    }
  }
}

printf("%f, %f, %f \n", 
  (double)cpersons/(double)(N_LINES * M_COLUMNS),
  (double)cdoctors/(double)(N_LINES * M_COLUMNS),
  (double)cvirus/(double)(N_LINES * M_COLUMNS)
);

show_grid(table, N_LINES, M_COLUMNS);

for (i=0; i<cpersons; i++){
  row = people[i].pos.y;
  column = people[i].pos.x;
  tloc = row*N_LINES + column;
  ploc = table[tloc].p - 1;
  printf("person[%d] is located at [%d,%d] ", i+1, row, column);
  if (ploc == i){
    printf("MATCH \n");
  }
  else {
    printf("MISMATCH \n");
  }
}

show_grid(table, N_LINES, M_COLUMNS);

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

  update_positions(persons, N);

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

  


int update_positions(float *persons, int N)
{
/*
 * For n = 0 ... N-1
 * Every person n makes a unit-length step towards the person n+1
 * (except person N-1, who steps towards person 0).
 */
float buffer[2], connector[2];
float distance;
int n;

buffer[0] = persons[0];
buffer[1] = persons[1];

/* All persons expect for the last one (N-1): */
for(n = 0; n < N-1; n++)
  {
  connector[0] = persons[2*(n+1)  ] - persons[2*n  ];
  connector[1] = persons[2*(n+1)+1] - persons[2*n+1];

  distance  = connector[0] * connector[0];
  distance += connector[1] * connector[1];
  distance  = sqrt(distance);

  if(distance > SMALLDISTANCE)
    {
    connector[0] /= distance;
    connector[1] /= distance;
    }
 
  persons[2*n  ] += connector[0];
  persons[2*n+1] += connector[1];
  }

/* Last person (number N-1): */
connector[0] = buffer[0] - persons[2*(N-1)  ];
connector[1] = buffer[1] - persons[2*(N-1)+1];

distance  = connector[0] * connector[0];
distance += connector[1] * connector[1];
distance  = sqrt(distance);

if(distance > SMALLDISTANCE)
  {
  connector[0] /= distance;
  connector[1] /= distance;
  }
 
persons[2*(N-1)  ] += connector[0];
persons[2*(N-1)+1] += connector[1];

return(n);
}
