#ifndef __d3yy_VISUALIZATION_H
#define __d3yy_VISUALIZATION_H


#include"libraries.h"

int allocate_SDL_pixelarray(struct SDL_graphics *graph);

int initialize_SDL_graphics(struct SDL_graphics *graph);

int initialize_pixel_array(struct SDL_graphics *graph);

int fade_pixel_array(struct SDL_graphics *graph, int fader);

int sdl_update(struct SDL_graphics *graph);

int visualise_person(struct SDL_graphics *graph, Person *p, int radius);

int visualize_person(struct SDL_graphics *graph, float *persons, int n, int radius);

int drawbox (struct SDL_graphics *graph, int left, int right, int top, int bottom, int width, int brightness);

int write_ppm(struct SDL_graphics *graph, char *filename);


#endif
