#ifndef __346k_SDLDATASTRUCTURE_H
#define __346k_SDLDATASTRUCTURE_H


#include <SDL/SDL.h>

struct SDL_graphics
  {
  SDL_Surface *screen;
  SDL_Surface *memfield;
  unsigned char *pixel;
  int width;      
  int height;     
  };


#endif
