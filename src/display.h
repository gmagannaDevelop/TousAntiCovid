#ifndef __2asjd293cc_DISPLAY_H
#define __2asjd293cc_DISPLAY_H


void show_grid(Case *table, int N, int M);

void show_grid_lists(
  Case *table, int N, int M,
  struct singly_linked_list *people,
  struct singly_linked_list *doctors
);

#endif