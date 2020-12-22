

#include "libraries.h"

void show_grid(Case *table, int N, int M)
{
  int row, column;
  Case *itable;
  int has_p, has_d, has_v;

  for (row=0; row<N; row++){
    printf("\n");
    for (column=0; column<M; column++){
      itable = &table[row*N + column];
      has_p = itable->p != FALSE;
      has_d = itable->d != FALSE;
      has_v = itable->viral_charge != FALSE;
      if(!has_p && !has_d && !has_v){
        printf(".");
      }
      else if(has_p){
        printf("λ");
      }
      else if(has_d){
        printf("D");
      }
      else if (has_v){
        printf("v");
      }
      else {
        printf("I HAVE FAILED YOU \n");
        exit(EXIT_FAILURE);
      }
    }
  }
  printf("\n");
}
