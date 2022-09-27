/* This function is used to store the string entered by the user into dynamic memory,
   alongside store the position of the last ever character in the string and
   also the position of the operator character in the list
 */

#include "apc.h"
Status_t is_operator(char ch);
/* Using dynamic memory allocation we are storing any length of the string and the address pointing to that string is stored in the pointer str which belongs to the main function */
Status_t store_expression(char **str, uint *str_end, uint *oper_index)
{
    FILE *fptr = stdin;
    uint size = SIZE_ALLOC_EACH_TIME;
    *str = malloc(sizeof(**str) * size);
    uint str_index = 0;
    char oper_check_flag = 'n';
    // ch taken as int instead of char as compiler dont have to make conversion to and from char to int and int to char.
    int ch;
    while((ch = fgetc(fptr)) != EOF && ch != '\n')
    {
	//for debuge; printf("test %c\n", ch);
	/* isdigit returns zero means it was ot a digit,
	   possibly could be a symbol */
	if(isdigit(ch) == 0)
	{
	    if(is_operator(ch) == success)
	    {
		if(*oper_index == 0)
		{
		    // debuge pusrpose; printf("test hi %u\n", *oper_index);
		    /* To be used in main function to know the operator in the string */
		    *oper_index = str_index;
		    oper_check_flag = 'y';
		}
		else
		{
		    /* free the allocated memory as no use if the expression is not valid 
		     as it contains more than one operators */
		    free(*str);
		    return failure;
		}
	    }
	    else
	    {
		/* free the allocated memory as no use if the expressin is not valid 
		 i.e. expressin does not cotain the valid operator */
		free(*str);
		/* this will be checked in the main file if string was not stored means as operator was found,
		   so that we can replace operator with '?' */
		*str = NULL;
		return failure;
	    }
	}
	// This simply means that we are dereferencing the str as we have to store the address returned by the malloc into the pointer which belongs to the main function.
	(*str)[str_index++] = ch;
	if(str_index == size)
	{
	    *str = realloc(*str, sizeof(**str) * (size += SIZE_ALLOC_EACH_TIME));
	    if(str == NULL)
		return failure;
	}

    }
    
    /* str_index representing the last end of the string where '\0' can be put */
    //str[str_index] = '\0';
    if(oper_check_flag == 'n')
    {
	free(*str);
	*str = NULL;
    }
    *str_end = str_index;
    

    return success;
}

/* It checks that only the valid operator is available in the list */
Status_t is_operator(char ch)
{
    if(ch == '+' || ch == '-' || ch == '*' || ch == '/')
    {
	return success;
    }
    else
    {
	return failure;
    }
}

