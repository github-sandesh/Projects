/* This function will be used to store the operator into the operator variable,
   Store the first operand into the list by conversting string to the corresponding value,
   store the second operand the same way
   */

#include "apc.h"
void reverse_string(char buffer[]);

Status_t create_list(Dlist_t **head, Dlist_t **tail, char *str, int start, int end)
{
    char buffer[5], ch; // = {'2', '4', '7'};
    /* end - 1 as end is pointing to '\0' or the operator 
     i indicates current index of the main user given string
     whereas j indicates the buffer index */
    uint i = end - 1, j = 0;
    while(i != start)
    {
	for(j = 0; j <= 3;)
	{
	    /* for Debug:  printf("%c ", ch); */
	    buffer[j++] = str[i--];
	    
	    if(i == start)
		break;
	}
	buffer[j] = '\0';
	// For Debug: printf("\nthe four char string is %s\n", buffer);
	reverse_string(buffer);
	// For Debug: printf("value is: %d\n", atoi(buffer));
	insert_at_first(head, tail, atoi(buffer));

    }

    return 0;
}

/* Reversing the strin is very much necessary as we are fetching the string from the end,
   so reversing it will give the original string in the variable buffer */
void reverse_string(char buffer[])
{
    uint len = strlen(buffer);
    // For Debug: printf("%u\n", len);
    /* len indexing the null character i.e. '\0' but we want only the no of useful characters hence decrement by one */
    --len;
    for(int i = 0; i <= len / 2; i++)
    {
	char temp = buffer[i];
	buffer[i] = buffer[len - i];
	buffer[len - i] = temp;
    }
}





