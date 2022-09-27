// includes the necessary header files and function prototypes.
#include"invertedIndex.h"
/* will update the database with the new files data */
int update_DB(Flist **f_head, Wlist **HT_head, char *filename)
{
    /* this variable will give the status about the files is valid or not */
    extern int  validate_status;
    // this array will store the filename at the index 1.
    // and last index contains NULL as its necessary to stop the traversing at last index in the read_filedata function.
    char *f_name[3];
    f_name[1] = filename;
    f_name[2] = NULL;

    // printf("file name in update is %s,  %s\n", f_name[1], filename);

    /* reset validate_status to false */
    validate_status = 0;
    /* calling this function will validate and store the file into the file list */
    // and return the files status into the validate_status variable.
    validate_n_store_filenames(f_head, f_name);

    /* If the file is invalid then stop */
    if(validate_status == 1)
    {
	return FAILURE;
    }
    else
    {
	/* If file list is empty means print the error */
	if(f_head == NULL)
	{
	    printf("\033[0;31mError validating the files\n");
	    return FAILURE;

	}
	/* call the function to read the file data and to store it in the data base */
	read_datafile(HT_head, filename);
	return SUCCESS;
    }

}

/* this function searches for the given word */
int search_DB(Wlist **head, char *word)
{
    //Finding the index number (hash function)
    int index = tolower(word[0]) % 97;

    // Cheking for non-alphabets
    if (index > 25)
	index = 26;

    //Creating the local reference
    Wlist *temp = head[index];

    /* this outer loop traverses the word list */
    while (temp)
    {
	/* if the given word is not found then continue traversing */
	if (strcmp(temp -> word, word))
	{
	    /* pointing to the next word node */
	    temp = temp -> link;
	}
	else
	{
	    // Print the message
	    printf("\033[0;32mWord \033[0;36m%s \033[0;32mis found at the Database and it is present in \033[0;31m%d \033[0;32mfile(s)\n", temp -> word, temp -> f_count);
	    //Create the local reference for file table
	    Tlink *f_temp = temp -> table_link;
	    /* this inner loop traverses throughout the table_link */
	    while (f_temp)
	    {
		/* Print the filename and its word count */
		printf("\033[0;32mIn file \033[0;36m%s \033[0;31m%d \033[0;32mtime(s)\n", f_temp -> f_name, f_temp -> w_count);
		/* point to the next table_link*/
		f_temp = f_temp -> link;
	    }
	    return SUCCESS;
	}
    }
    /* If traversing through all the word links and tables links, if word is not found print the error message */
    printf("\033[0;31mThe word \033[0;36m%s \033[0;31mis not found at the database\n", word);

}
