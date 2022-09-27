/*******************************************************************************************************************************************************************
*Title			: Addition
*Description		: This function performs addition of two given large numbers and store the result in the resultant list.
*Prototype		: int addition(Dlist_t **head1, Dlist_t **tail1, Dlist_t **head2, Dlist_t **tail2, Dlist_t **headR);
*Input Parameters	: head1: Pointer to the first node of the first double linked list.
			: tail1: Pointer to the last node of the first double linked list.
			: head2: Pointer to the first node of the second double linked list.
			: tail2: Pointer to the last node of the second double linked list.
			: headR: Pointer to the first node of the resultant double linked list.
*Output			: Status (SUCCESS / FAILURE)
*******************************************************************************************************************************************************************/
#include "apc.h"

int addition(Dlist_t *head1, Dlist_t *tail1, Dlist_t *head2, Dlist_t *tail2, Dlist_t **headR, Dlist_t **tailR)
{
    int carry = 0, res, opr1, opr2;

    while(tail1 != NULL || tail2 != NULL)
    {
	if(tail1 == NULL)
	{
	    opr1 = 0;
	}
	else
	{
	    opr1 = tail1 -> value;
	    tail1 = tail1 -> prev;
	}
	if(tail2 == NULL)
	{
	    opr2 = 0;
	}
	else
	{
	    opr2 = tail2 -> value;
	    tail2 = tail2 -> prev;
	}

	res = opr1 + opr2 + carry;
	carry = res / 10000;
	//printf("%d ", res % 10000);

	insert_at_first(headR,tailR, res % 10000);

    }
    if(carry)
    {
	insert_at_first(headR, tailR,  carry);
    }
    return 0;
}
