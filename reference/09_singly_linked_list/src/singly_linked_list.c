/*
 * The singly linked list which is implemented here
 * will always end with a node called 'end cap'.
 * The end cap has 'content' = 0 and 'next' = NULL.
 * The 0 'content' of the end cap is not counted as
 * a content of the list.
 *
 * NOTE: The singly linked list must be allocated
 * by the function 'allocate_and_initialize_sll()'.
 * Otherwise, the 'remove_content_from_sll()'
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

(*sll)->content = 0;  /* This is equivalent to (*(*sll)).content */
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
  printf("%d\n", sll->content);
  print_sll(sll->next);
  }

return(1);
}



int extend_sll(struct singly_linked_list *sll, int content)
{
/*
 * Example behaviors for 'content' = 17:
 * [0]->NULL  is changed to  [17]->[0]->NULL 
 * [3]->[0]->[1]->[0]->NULL is changed to [3]->[0]->[1]->[17]->[0]->NULL
 *
 */
if(NULL != sll->next)
  {
  extend_sll(sll->next, content);
  }
else
  {
  sll->next = (struct singly_linked_list *)malloc(sizeof(struct singly_linked_list));
  if(NULL == sll->next)
    {
    printf("\n\nAllocation error in function 'extend_sll()'\n\n");
    exit(0);
    }
  sll->content = content;
  sll->next->content = 0;
  sll->next->next =NULL;
  }

return(1);
}



struct singly_linked_list *extend_sll_return_lastnode\
         (struct singly_linked_list *sll, int content)
{
/*
 * Same as 'extend_sll', but also returns a pointer
 * to the last node of the sll before the end cap node.
 *
 */
if(NULL != sll->next)
  {
  return(extend_sll_return_lastnode(sll->next, content));
  }
else
  {
  sll->next = (struct singly_linked_list *)malloc(sizeof(struct singly_linked_list));
  if(NULL == sll->next)
    {
    printf("\n\nAllocation error in function 'extend_sll()'\n\n");
    exit(0);
    }
  sll->content = content;
  sll->next->content = 0;
  sll->next->next =NULL;
  return(sll);
  }
}
  


int extend_sll_at_head(struct singly_linked_list **sll, int content)
{
/*
 * Example behaviors for 'content' = 17:
 * [0]->NULL  is changed to  [17]->[0]->NULL 
 * [3]->[0]->[1]->[0]->NULL is changed to [17]->[3]->[0]->[1]->[0]->NULL
 * The return value is 'content'.
 *
 */
struct singly_linked_list *tmp_listptr;
int tmp_content = 0;

tmp_content = (*sll)->content;
tmp_listptr = (*sll)->next;

free(*sll);
allocate_and_initialize_sll(sll);
  
extend_sll(*sll, content);
extend_sll(*sll, tmp_content);

free((*sll)->next->next->next);
free((*sll)->next->next);
(*sll)->next->next = tmp_listptr;

return(content);
}



int is_in_sll(struct singly_linked_list *sll, int content)
{
if(NULL != sll->next)
  {
  if(content == sll->content)
    {
    return(1);
    }
  else
    {
    return(is_in_sll(sll->next, content));
    }
  }

return(0);
}



int remove_content_from_sll(struct singly_linked_list *sll, int content)
{
int buffercontent;
struct singly_linked_list *bufferptr;

if(NULL != sll->next) /* Test that sll is not the end cap node of the list */ 
  {
  if(content == sll->content)
    {
    buffercontent = sll->next->content;
    bufferptr = sll->next->next;
    free(sll->next);
    sll->content = buffercontent;
    sll->next = bufferptr;
    return(1);
    }
  else
    {
    return( remove_content_from_sll(sll->next, content) );
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
 * Removes the last node with actual content (not the cap node)
 * and returns the content that was stored in that node before
 * it got removed.
 *
 */
int content = 0;

if(NULL == sll->next)
  {
  return(sll->content);
  }

if(NULL == sll->next->next)
  {
  content = sll->content;
  free(sll->next->next);
  free(sll->next);
  sll->content = 0;
  sll->next = NULL;
  return(content);
  }

if(NULL != sll->next->next)
  {
  return(remove_last_node_from_sll(sll->next));
  }

return(0); /* This return() should never be reached. */
}
