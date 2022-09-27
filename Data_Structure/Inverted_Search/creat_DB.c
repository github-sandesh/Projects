// contains neccessary header files and function prototypes.
#include "invertedIndex.h"

/* To store the filenames */
char Filename[FNAME_SIZE];
char *ext_g;
/* Will create the database */
int create_DB(Flist *file_head, Wlist **head)
{
    /* Run loop till the last file in the list */
    while (file_head != NULL)
    {
	/* Reads the words from the selected file */
	read_datafile(head, file_head -> filename);
	/* point to the next file */
	file_head = file_head -> link;
    }
}

/* to update the word nodes info in case the word matches with the already existing database */
int update_word_count(Wlist **head, char *filename)
{
    /*TODO:
      1. Create local reference for link table
      2. traverse throgh link table and compare the filenames (repeat until reach last node)
      2.1 if the filenames are same, increment the word count and break
      2.2 if the filenames are not same and traverse to next node,
      2.3 then increment filecount in Wlist
      2.4 create link table and update the word count = 1, new filename and link = NULL
      2.5 Establish link b/w last node and the new of link table
      3. Return SUCCESS;
     */

    /* temporory variable to store the word list head */
    Tlink *temp = (*head) -> table_link;
    /* Traverse till the last node */
    while(temp -> link != NULL)
    {
	/* If this word is present in any other file then update word count of that files */
	if(temp -> f_name == filename)
	{
	    ++(temp -> w_count);
	    return SUCCESS;

	}
	// points to the next node.
	temp = temp -> link;

    }
    /* now temp is pointing to the last node
       so check and increment word count here also if the last file has that word */
    //if(temp -> f_name == filename)
    if(strcmp(temp -> f_name, filename) == 0)
    {
	++(temp -> w_count);
	return SUCCESS;

    }
    /* this is when this word is not present in any files under this word node */
    else
    {
	/* So create now link node to store the new filename */
	Tlink *new = malloc(sizeof(Tlink));
	if(new == NULL)
	{
	    return FAILURE;

	}

	/* Update the table node link to NULL, w count to 1 and filename */
	++((*head) -> f_count);
	new -> link = NULL;
	new -> w_count = 1;
	strcpy(new -> f_name, filename);
	/* point to the next table node */
	temp -> link = new;

	return SUCCESS;

    }

}

/* This function will read the words from the file one by one and stores into datafbase */
void read_datafile(Wlist **head, char *f_name)
{
    // we can use either this or
    // extern char Filename[];
    // strcpy(Filename, file_head -> filename);

    extern char *ext_g;
    ext_g = f_name;
    // printf("%s\n", ext_g);

    /* to store pointer of file opened. */
    FILE *fptr = fopen(f_name, "r");
    /* buffer to store the word tempororily */
    char word[NAME_LENGTH];
    short index, flag;

    /* scan the word from the files one by one till the end of the file */
    while (fscanf(fptr, "%s", word) != EOF)
    {
	// flag to indicate no matching found for the already existing words.
	flag = 1;

	// Converting alphabets to lowercase and finding the index number.
	index = tolower(word[0]) % 97;

	// printf("\033[0;33m index is = %d : %d", tolower(word[0]), index);
	// Checking for non-albhabet characters
	/* If symbols or digits are there will be stored at the 26 index */
	if (index > 25)
	{
	    index = 26;
	}

	/* Traverse till the end of the word list */
	if (head[index] != NULL)
	{
	    // Local reference for head[index]
	    Wlist *temp = head[index];
	    while (temp)
	    {
		/* While traversing check each word if, it repeated
		   then just update its word count else creat new word node */
		if (strcmp(temp -> word, word) == 0)
		{
		    /* this function will increment the word count in the word node. */
		    update_word_count(&temp, f_name);
		    // set flag to zero if word is found.
		    flag = 0;
		    break;
		}
		// Point to the next node word node.
		temp = temp -> link;
	    }

	}
	/* if the word is not found in all the word nodes then insert new node at the last */
	if (flag)
	    // here one thing that we can do is we can make the temp point to the last node.
	    // and then use that address to store the address of new word node into it instead of traversing again just to update the value of the link into the word node.
	    insert_at_last_main(&head[index], word);

    }
    /* files is to be closed as its use is over now */
    fclose(fptr);
    // Print succdess message.
    printf("\033[0;32mSuccessfull : Creation of database for file \033[0;36m%s\n", f_name);

}

/* will insert word node at the end of the word list */
int insert_at_last_main(Wlist **head, char *word)
{
    /* create and validate the new node */
    Wlist *new = malloc(sizeof(Wlist));
    if (new == NULL)
    {
	return FAILURE;
    }

    /* Update the word and the file count
       and alsoupdate the link with NULL */
    strcpy(new -> word, word);
    new -> f_count = 1;
    new -> link = NULL;

    /* this function will update the link table node */
    update_link_table(&new);

    // Check the list is empty
    if (*head == NULL)
    {
	/* If the head is NULL means list is empty so update head with new node */
	*head = new;
	return SUCCESS;
    }

    //List is not empty
    /* TODO:
       1. create local reference for head
       2. run the loop until temp reaches last node
       3. Establish the link b/w last and new node
       4. return SUCCESS
     */

    /* local head for traversing */
    Wlist *temp = *head;
    /* Traverse till the last node */
    while(temp -> link != NULL)
    {
	// point to the next node.
	temp = temp -> link;

    }
    /* establish the link between the new node and the last node,
       since temp is now pointing to the last node */
    temp -> link = new;
    return SUCCESS;

}

int update_link_table(Wlist **W_node)
{
    // NOTE: Not collecting the filename, So make global variable
    /* TODO: 
       1. Create a node type of Tlink
       2. Check the node is created or not
       3. if created update the filename, word count = 1, link = NULL
       4. update address of Tlink node with W_node -> table_link.
       5. retuirn SUCCESS
     */

    //printf("\nextern filename is %s ", ext_g );

    /* create and validate the new node */
    Tlink *new = malloc(sizeof(Tlink));
    if(new == NULL)
    {
	return FAILURE;
    }
    /* ext_g contains the filename */
    extern char *ext_g;
    // update the filename, word count.
    // update the link to NULL.
    strcpy(new -> f_name, ext_g);
    new -> w_count = 1;
    new -> link = NULL;
    /* point to the next word node */
    (*W_node) -> table_link = new;
    return SUCCESS;

}

