/*
 * The singly linked list which is implemented here
 * will always end with a node called 'end cap'.
 * The end cap has 'id' = 0 and 'next' = NULL.
 * The 0 'id' of the end cap is not counted as
 * a id of the list.
 *
 * NOTE: The singly linked list must be allocated
 * by the function 'allocate_and_initialize_sll()'.
 * Otherwise, the 'remove_id_from_sll()'
 * function may cause a runtime error by calling
 * free() on a non previously allocated sll. 
 *
 */

#include "libraries.h"


struct singly_linked_list *allocate_and_initialize_sll\
                      (struct singly_linked_list **sll)
{
*sll = (struct singly_linked_list *)malloc(sizeof(struct singly_linked_list));
if(NULL == *sll)
  {
  printf("\n\nAllocation error in function"\
         " 'allocate_and_initialize_sll()'\n\n");
  exit(0);
  }

(*sll)->id = 0;  /* This is equivalent to (*(*sll)).id */
(*sll)->next =NULL;
return(*sll);
}



int sll_list_length(struct singly_linked_list *sll)
{
if(NULL != sll->next)
  {
  return(1 + sll_list_length(sll->next));
  }
return(0);
}



int print_sll(struct singly_linked_list *sll)
{
if(NULL == sll->next)
  {
  printf("End of list.\n");
  }
else
  {
  printf("%d\n", sll->id);
  print_sll(sll->next);
  }

return(1);
}



int extend_sll(struct singly_linked_list *sll, int id)
{
/*
 * Example behaviors for 'id' = 17:
 * [0]->NULL  is changed to  [17]->[0]->NULL 
 * [3]->[0]->[1]->[0]->NULL is changed to [3]->[0]->[1]->[17]->[0]->NULL
 *
 */
if(NULL != sll->next)
  {
  extend_sll(sll->next, id);
  }
else
  {
  sll->next = (struct singly_linked_list *)malloc(sizeof(struct singly_linked_list));
  if(NULL == sll->next)
    {
    printf("\n\nAllocation error in function 'extend_sll()'\n\n");
    exit(0);
    }
  sll->id = id;
  sll->next->id = 0;
  sll->next->next =NULL;
  }

return(1);
}



struct singly_linked_list *extend_sll_return_lastnode\
         (struct singly_linked_list *sll, int id)
{
/*
 * Same as 'extend_sll', but also returns a pointer
 * to the last node of the sll before the end cap node.
 *
 */
if(NULL != sll->next)
  {
  return(extend_sll_return_lastnode(sll->next, id));
  }
else
  {
  sll->next = (struct singly_linked_list *)malloc(sizeof(struct singly_linked_list));
  if(NULL == sll->next)
    {
    printf("\n\nAllocation error in function 'extend_sll()'\n\n");
    exit(0);
    }
  sll->id = id;
  sll->next->id = 0;
  sll->next->next =NULL;
  return(sll);
  }
}
  


int extend_sll_at_head(struct singly_linked_list **sll, int id)
{
/*
 * Example behaviors for 'id' = 17:
 * [0]->NULL  is changed to  [17]->[0]->NULL 
 * [3]->[0]->[1]->[0]->NULL is changed to [17]->[3]->[0]->[1]->[0]->NULL
 * The return value is 'id'.
 *
 */
struct singly_linked_list *tmp_listptr;
int tmp_id = 0;

tmp_id = (*sll)->id;
tmp_listptr = (*sll)->next;

free(*sll);
allocate_and_initialize_sll(sll);
  
extend_sll(*sll, id);
extend_sll(*sll, tmp_id);

free((*sll)->next->next->next);
free((*sll)->next->next);
(*sll)->next->next = tmp_listptr;

return(id);
}



int is_in_sll(struct singly_linked_list *sll, int id)
{
if(NULL != sll->next)
  {
  if(id == sll->id)
    {
    return(1);
    }
  else
    {
    return(is_in_sll(sll->next, id));
    }
  }

return(0);
}



int remove_id_from_sll(struct singly_linked_list *sll, int id)
{
int bufferid;
struct singly_linked_list *bufferptr;

if(NULL != sll->next) /* Test that sll is not the end cap node of the list */ 
  {
  if(id == sll->id)
    {
    bufferid = sll->next->id;
    bufferptr = sll->next->next;
    free(sll->next);
    sll->id = bufferid;
    sll->next = bufferptr;
    return(1);
    }
  else
    {
    return( remove_id_from_sll(sll->next, id) );
    }
  }

return(0);
}



int empty_sll(struct singly_linked_list *sll)
{
while(NULL != sll->next)
  {
  remove_last_node_from_sll(sll);
  }
return(1);
}



int remove_last_node_from_sll(struct singly_linked_list *sll)
{
/*
 * Removes the last node with actual id (not the cap node)
 * and returns the id that was stored in that node before
 * it got removed.
 *
 */
int id = 0;

if(NULL == sll->next)
  {
  return(sll->id);
  }

if(NULL == sll->next->next)
  {
  /* save id : */
  id = sll->id;
  /* call free(NULL) */
  free(sll->next->next);
  /* call free on next node */
  /* SHOULD CALL FREE ON `next_node->p` FIRST ! */
  free(sll->next);
  sll->id = 0;
  sll->next = NULL;
  return(id);
  }

if(NULL != sll->next->next)
  {
  return(remove_last_node_from_sll(sll->next));
  }

return(0); /* This return() should never be reached. */
}
