/* has necessary header files and prototypes */
#include "invertedIndex.h"
/* displays all the database */
int display_DB(Wlist **head)
{

    /* flag variable */
    char flag;

    /* This outer loop traverses throughout the hash table */
    for (int i = 0; i < 27; i++)
    {
	/* reset the flag to the no i.e. false for every word list */
	flag = 'n';
	// Checking for non-empty index
	if (head[i] != NULL)
	{
	    //This is non-empty index and printing its value.
	    printf("\033[0;34m[%d]\t", i);
	    //Format: [word] <filecount> time(s): File <filename>: <wordcount> time(s)
	    /* 
	    TODO: 
	    1. Create local reference for Wlist
	    2. Run loop and traverse till NULL
	    2.1 print the word, filecount in given format
	    2.2 create local reference for file table 
	    2.3 Run loop till NULL
	    2.3.1 print the data in given format
			 */
	    /* Temporory word list pointer */
	    Wlist *wtemp = head[i];
	    /* Temporory table link pointer */
	    Tlink *ttemp;
	    /* This middle loop will traverse throughout the word list */
	    while(wtemp)
	    {
		// Modify the flag to true i.e. yes
		// so that \t is not printed for only the first iteration.
		if(flag == 'n'){
		    flag = 'y';
		}
		else{
		    printf("\t");
		}

		// strlen(wtemp -> word);
		/* Print the word count and the file count */
		printf("\033[0;32m[%s]%*s \033[0;31m%d \033[0mfile(s) ", wtemp -> word, MAX_WORD_SIZE - (int) strlen(wtemp -> word), " ",  wtemp -> f_count);
		/* Initialise the table link from the word node */
		ttemp = wtemp -> table_link;
		/* inner loop traverses throughout all the table link node to print the files and their word count */
		while(ttemp)
		{
		    /* Prints the file name and corresponding word counts */
		    printf(" : File : \033[0;32m%-10s  \033[0;31m%d \033[0mtime(s)", ttemp -> f_name, ttemp -> w_count);
		    /* Point to the next table link node */
		    ttemp = ttemp -> link;

		    /* Print NULL for the last node only */
		    if(ttemp == NULL){
			printf(" -> NULL");
		    }

		}
		/* Point to the next word node */
		wtemp = wtemp -> link;
		printf("\n");

	    } 

	}
    }
}

/* this function will save the database into the given file */
int save_DB(Wlist **head, Flist *f_head, char *save_file)
{
    /* File pointr to store the file address */
    FILE *fptr;
    char flag;

    /* open the backup file in the write mode */
    fptr = fopen(save_file, "w");
    if(fptr == NULL)
    {
	printf("\033[0;31mFailed to create or open the file \033[0;34m%s\n", save_file); 
	return FAILURE;
    }

    /* Outer loop traverses throughout the hash table */
    for (int i = 0; i < 27; i++)
    {
	// Checking for non-empty index
	if (head[i] != NULL)
	{
	    //This is non-empty index and printing the data
	    fprintf(fptr, "#%d;\n", i);
	    //Format: [word] <filecount> time(s): File <filename>: <wordcount> time(s)
	    /*
	    TODO: 
	    1. Create local reference for Wlist
	    2. Run loop and traverse till NULL
	    2.1 print the word, filecount in given format
	    2.2 create local reference for file table 
	    2.3 Run loop till NULL
	    2.3.1 print the data in given format
	    */
	    /* local reference for the word list */
	    Wlist *wtemp = head[i];
	    /* local pointer for the table link */
	    Tlink *ttemp;
	    /* Middle loop traverses throughout the word list */
	    while(wtemp)
	    {
		// strlen(wtemp -> word);
		/* Print the word and the filecount into the file */
		fprintf(fptr, "%s;%d;", wtemp -> word, wtemp -> f_count);
		/* Initialise the table link pointer */
		ttemp = wtemp -> table_link;
		/* Inner loop traverses throughout the table link */
		while(ttemp)
		{
		    /* Print the filename and its word count into the backup file */
		    fprintf(fptr, "%s;%d", ttemp -> f_name, ttemp -> w_count);
		    /* point to the next link */
		    ttemp = ttemp -> link;
		    /* Print # at the end of the list */
		    if(ttemp == NULL){
			fprintf(fptr, ";#");
		    }
		    // else print ; every time.
		    else{
			fprintf(fptr, ";");
		    }

		}
		/* Point to the next node of the word list */
		wtemp = wtemp -> link;
		fprintf(fptr, "\n");
	    } 
	}
    }
    /* close the file */
    fclose(fptr);
    /* Print the success message */
    printf("\033[0;32mSuccessfull : Database saved in \033[0;36m%s \033[0;32mfile\n", save_file);

}
