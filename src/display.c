

#include "libraries.h"

void print_case(Case c)
{
  printf("p(%d),d(%d),viral_charge(%d),danger(%d)\n",
    c.p, c.d, c.viral_charge, c.danger
  );
}

void print_person(Person p)
{
  printf("alive : %d\n", p.alive);
  printf("viral_charge : %d\n", p.viral_charge);
  printf("(y, x) = (%d, %d)\n", p.pos.y, p.pos.x);
  printf("direction : %d\n", p.direction);
  printf("healing : %d\n\n", p.healing);
}


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
