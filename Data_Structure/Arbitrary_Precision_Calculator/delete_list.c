
/* This function helps in deleting all the three lists before we start to use these lists for the next operation */
#include "apc.h"

int delete_list(Dlist_t **head, Dlist_t **tail)
{
    /* If the list is empty means no need to delete anything */
    if(*head == NULL)
	SUCCESS;
    /* Traverse using current_node */
    Dlist_t *current_node = *head;
    /* To keep the backup of the current_node, which will get updated to the next node */
    Dlist_t *temp;
    /* Traverse till the last node */
    while(current_node != NULL)
    {
	/* backup the curr node */
	temp = current_node;
	/* Point to the next node */
	current_node = current_node -> next;
	/* Delete the node */
	free(temp);
    }
    /* Replace both the head and tail with NULL */
    *head = NULL;
    *tail = NULL;

    return 0;
}

