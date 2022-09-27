/**************************************************************************************************************************************************************
 *Title		: main function(Driver function)
 *Description	: This function is used as the driver function for the all the functions
 *Note		: We have to make sure that the scanning and storing of the strings is done from the last end,
 		  as it makes sure that the proper alignment as we always do addition from the unit place.
 ***************************************************************************************************************************************************************/
#include "apc.h"

int main(int argc, char *argv[])
{
    /* Declare the pointers */
    Dlist_t *head1 = NULL, *tail1 = NULL, *head2 = NULL, *tail2 = NULL, *headR = NULL , *tailR = NULL;
    char option, operator = 0, *str = NULL;
    /* str_end is what holds the index of the last character i.e. '\0' */
    uint str_end = 0, oper_index = 0;
    char buffer[5];
/*
    if(validate_cla(argc) == SUCCESS)
    {
	printf("Error: Please pass minimum of one argument\n e.g. 1234+56789\n");
	return FAILURE;
    }
    */

    printf("\033[0;33mUsage:  12354+123566456\n\t789654-9898565213\n");

    do
    {
	/* Deleting the old list as its of no use now and new list to be created */
	delete_list(&head1, &tail1);
	delete_list(&head2, &tail2);
	delete_list(&headR, &tailR);
	oper_index = 0;
	/* Code for reading the inputs */

	printf("\033[0;33mEnter the Expression: \033[0;36m");
	__fpurge(stdin);
	if(store_expression(&str, &str_end, &oper_index))
	{
	    printf("\033[0;31mError : Either expression is not valid\n\tor the dynamic memory allocation has failed\n");
	}
	// for debugging; to check whether the string is ready or not; printf("string is: %s\n", str);
	if(str == NULL)
	{
	    printf("\033[0;31mError:  NO operator was found,\n\tPlease pass the valid operator\n");
	    goto skip_this_iter_if_no_oper;
	}
	else
	{
	    operator = str[oper_index];
	}

	printf("\033[0;31m**************************** INFO! *********************************************\n\033[0;33mTotal number of characters entered by user are: \033[0;36m%d\n\033[0;33moperator entered by user is: \033[0;36m'%c' \033[0;33mlocated at index: \033[0;36m[%d]\n\033[0;33mInput given by the user is: \033[0;36m%s\n\033[0;31m********************************************************************************\n",str_end, operator, oper_index, str);
	
	/* To create the first list */
	create_list(&head2, &tail2, str, oper_index, str_end);
	/* Create the second list */
	create_list(&head1, &tail1, str, -1, oper_index);
	/* print the first list for confirmation */
	printf("\n\033[0;33mThe first operand is: \033[0;36m");
	print_list(head1);
	/* print the second list for confirmation */
	printf("\033[0;33mThe second operand is: \033[0;36m");
	print_list(head2);

	/* Function for extracting the operator */

	switch (operator)
	{
	    case '+':
		/* call the function to perform the addition operation */
		addition(head1, tail1, head2, tail2, &headR, &tailR);
		printf("\n\033[0;33mThe Result of addition is: \033[0;36m");
		break;
	    case '-':	
		/* Here the idea is to subtact the bigger number from the smallest one
		and if the second number in list2 is bigger simply assign the negative sign to the result */
		/* the subtraction function has been coded such a way that it can only subtract the smaller list2 from the bigger list1 */
		{
		    int list1_nodes = (oper_index / 4);
		    if(oper_index % 4 != 0)
		    {
			list1_nodes++;
		    }
		    int no_of_char_in_list2 = str_end - oper_index - 1;
		    int list2_nodes = (no_of_char_in_list2 / 4);
		    if(no_of_char_in_list2 % 4 != 0)
		    {
			list2_nodes++;
		    }
		    
		    /* Now we have the details of the no of characters in list1 and in list2 */
		    if(list1_nodes > list2_nodes)
		    {
			subtraction(head1, tail1, head2, tail2, &headR, &tailR);
		    }
		    else if(list2_nodes > list1_nodes)
		    {
			subtraction(head2, tail2, head1, tail1, &headR, &tailR);
			headR -> value *= (-1);
		    }
		    /* list1 or list2 are not greater or smaller means they are equal,
		       so in this case we compare their head values only as Most significant digits always decides which number is greater */
		    else if(head1 -> value > head2 -> value)
		    {
			subtraction(head1, tail1, head2, tail2, &headR, &tailR);
		    }
		    else if(head2 -> value > head2 -> value)
		    {
			subtraction(head2, tail2, head1, tail1, &headR, &tailR);
			headR -> value *= (-1);
		    }
		    printf("\n\033[0;33mThe Result of Subraction is: \033[0;36m");
		}
		
		break;
	    case '*':	
		/* call the function to perform the multiplication operation */
		printf("\033[0;31mSorry!! Multiplication implementation is pending\033[0;36m");
		printf("\n\033[0;33mThe Result of Multiplication is: \033[0;36m");
		break;
	    case '/':	
		/* call the function to perform the division operation */
		printf("\033[0;31mSorry!! Division implementaion is pending\033[0;36m");
		printf("\n\033[0;33mThe Result of Division is: \033[0;36m");
		break;
	    default:
		printf("\n\033[0;31mError: Invalid Input:-( Try again...\n\033[0;33mUsage:  12354+123566456\n\t789654-9898565213\n");
		return -1;
	}
	/* This is to print the Result of the operation */
	print_list(headR);

	skip_this_iter_if_no_oper:

	printf("\n\033[0;37m----------------------------------------------------------------------------------------\n\033[0;33mWant to continue? Press \033[0;36m[yY | nN]: ");
	scanf("\n%c", &option);
    }while (option == 'y' || option == 'Y');

    return 0;
}



