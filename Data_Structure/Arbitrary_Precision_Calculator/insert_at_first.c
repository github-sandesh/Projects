/* This function will insert the new node at first position in the list */
#include "apc.h"

int insert_at_first(Dlist_t **head, Dlist_t **tail, int value)
{
    /* Create a new node */
    Dlist_t *new = malloc(sizeof(Dlist_t));
    if(new == NULL)
	return FAILURE;

    new -> value = value;
    new -> prev = NULL;
    /* If the list is empty insert first node here only */
    if(*head == NULL && *tail == NULL)
    {
	new -> next = NULL;
	*head = new;
	*tail = new;
	return SUCCESS;

    }

    /* Insert new node at first position */
    new -> next = *head;
    (*head) -> prev = new;
    *head = new;

    return 0;

}
