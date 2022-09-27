/* This function will print printing the both operand list and the result list */
#include"apc.h"

int print_list(Dlist_t *head)
{
    /* If the list is empty print NULL */
    if(head == NULL)
    {
	printf("NULL\n");
	return SUCCESS;
    }
    /* If the list is not empty then traverse throught the list and print each node data,
       But strictly to be printed in the four Digits as its the four digit number anyways */
    while(head != NULL)
    {
	printf("%4.4d ", head -> value);
	head = head -> next;
    }
    printf("\n");

    return SUCCESS;
}

