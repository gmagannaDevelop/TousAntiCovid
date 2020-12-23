

#include "libraries.h"

void show_grid(Case *table, int N, int M)
{
  int row, column;
  Case *itable;

  for (row=0; row<N; row++){
    printf("\n");
    for (column=0; column<M; column++){
      itable = &table[row*M + column];
      if((NULL == itable->p) && (itable->viral_charge == 0)){
        printf(".");
      }
      else if(NULL != itable->p){
        if (TRUE == itable->p->healing){
          printf("D");
        }
        else {
          printf("λ");
        }
      }
      else if (itable->viral_charge > 0){
        printf("v");
      }
    }
  }
  printf("\n");
}

void show_grid_lists(Case *table, int N, int M,
  struct singly_linked_list *people,
  struct singly_linked_list *doctors
){
  int row, column;
  Case *itable;

  for (row=0; row<N; row++){
    printf("\n");
    for (column=0; column<M; column++){
      itable = &table[row*M + column];
      if((NULL == itable->p) && (itable->viral_charge == 0)){
        printf(".");
      }
      else if(NULL != itable->p){
        if (TRUE == itable->p->healing){
          printf("D");
        }
        else {
          if (is_in_sll(people, itable->p)) {
            printf("Λ");
          }
          else {
            printf("λ");
          }
        }
      }
      else if (itable->viral_charge > 0){
        printf("v");
      }
    }
  }
  printf("\n");
}