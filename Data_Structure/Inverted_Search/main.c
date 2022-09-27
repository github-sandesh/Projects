/*	 Documentation
	 Name : Sandesh.
	 Date : 18/07/2022
  Description : Inverted Search
 Sample input :	./a.out file.txt file1.txt file3.txt
Sample output :	
		./a.out file.txt file1.txt file3.txt
		file.txt  validated successfully
		file1.txt  validated successfully
		file3.txt  validated successfully
		1. Create Database
		2. Display Database
		3. Search Database
		4. Update Database
		5. Save Database
		Please enter your choice : 1
		Successfull : Creation of database for file file.txt
		Successfull : Creation of database for file file1.txt
		Successfull : Creation of database for file file3.txt
		do you want to continue y/Y : y
		1. Create Database
		2. Display Database
		3. Search Database
		4. Update Database
		5. Save Database
		Please enter your choice : 2
		[7]     [hi]            2 file(s)  : File : file.txt    1 time(s) : File : file3.txt   1 time(s) -> NULL
	        [hello]         1 file(s)  : File : file1.txt   1 time(s) -> NULL
		[14]    [okay]          1 file(s)  : File : file1.txt   1 time(s) -> NULL
		[19]    [thank]         1 file(s)  : File : file3.txt   1 time(s) -> NULL
		[24]    [you]           2 file(s)  : File : file.txt    1 time(s) : File : file3.txt   1 time(s) -> NULL
		do you want to continue y/Y : n
 */


/*** If we dont write the code in the block inside the case then we end up getting an error called as,
  "" a label can only be part of a statement and a declaration is not a statement char *file; ""  */
#include<stdio.h>
#include"invertedIndex.h"
#include<unistd.h>
#include<stdio_ext.h>
// Global variable declaration
char flag = 'n';
char flag_once = 1;
int validate_status = 0;

int main(int argc,char * argv[])
{
    // check the argumets are passed.
    if (argc == 1)
    {
	printf("\033[0;31m");
	printf("No argumets are passed as filenames: Please pass the arguments\n");
    }
    // head pointer to create list of files.
    Flist *f_head = NULL;

    /* function to validate the files. */
    validate_n_store_filenames(&f_head, argv);

    // check that the file list is not empty means that no file was successfully validated.
    if(f_head == NULL)
    {
	printf("\033[0;31mError validating the files\n\033[0;33mUsage: ./a.out file1.txt file2.txt\n");
	return FAILURE;
    }
    /*********** this is for reference to know that the file names saved in the list
      Flist *temp = f_head;
      while(temp)
      {
      printf("%s\n", temp -> filename);
      temp = temp -> link;
      }
     ***************************************************/

    /* Display menu to the user
       1. Create Database
       2. Display Database
       3. search Database
       4. Update Database
       5. Save Database
     */


    // this is the array of the pointers.
    // its **(HT_head + i);
    Wlist *HT_head[27] = {NULL};

    // ask usr and take a choice from user.
    char ch = 'y';

    /* repeat the process till the user wants to */
    while(ch == 'y' || ch == 'Y')
    {
	int choice = 0;
	/* Print menus for user reference */
	printf("\033[0;34m");
	printf("1. Create Database\n2. Display Database\n3. Search Database\n4. Update Database\n5. Save Database\n");
	printf("\033[0m");

	/* Ask user the choice */
	printf("Please enter your choice : ");
	__fpurge(stdin);
	scanf("%d", &choice);

	/* Takeaction based on the user choice. */
	switch(choice)
	{
	    case 1 :
		// no need to use &HT_head as HT_head is already an address.i
		// passing only f_head not &f_head as we are reading those filenames only, not modifying any data at this address.
		/* Create Database only once for the files passed through command line. */
		if(flag_once)
		{
		    /* this creates the database for the files passed as cla */
		    create_DB(f_head, HT_head);
		    flag_once = 0;
		}
		break;

	    case 2 :
		/* Display the Database */
		display_DB(HT_head);
		break;
	    case 3 :
		{
		    char word[MAX_WORD_SIZE];
		    printf("Enter the word you want to search : ");
		    __fpurge(stdin);
		    scanf("%s", word);
		    /* To search the word in the database. */
		    search_DB(HT_head, word);
		}
		break;
	    case 4 :
		{
		    // Update Database
		    // pass the filename and then store its data into the database.
		    char file[MAX_FILE_SIZE];
		    printf("Enter the filename for which database is to be created : ");
		    __fpurge(stdin);
		    scanf("%s", file);
		    // need to pass the address of the f_head as we might have to edit the original file pointer.
		    /* To update the database from given file */
		    update_DB(&f_head, HT_head, file);
		}
		break;
	    case 5 :{
			// Save Database
			char save_file[MAX_FILE_SIZE];
			printf("Please enter the backup filename : ");
			scanf("%s", save_file);
			/* Saves the database into the external file */
			save_DB(HT_head, f_head, save_file);
		    }
		    break;
	    default :
		    printf("\033[0;31mPlease enter the valid choice : \n");
		    break;
	}

	/* To repeat any operation */
	printf("\033[0;33mdo you want to continue y/Y : ");
	__fpurge(stdin);
	scanf("%c", &ch);
	//ch = getc(stdin);

    }
    return 0;

}

/* Validates the files */
void validate_n_store_filenames(Flist **f_head, char *filenames[])
{
    /* index and status to store the return status of the function */
    int i = 1, status;

    // Run loop for no. of files times.
    while(filenames[i] != NULL)
    {
	printf("\033[0;36m");

	/* function call to check file status empty or nont exist */
	status = IsFileValid(filenames[i]);

	/* based on the return status print the the error messages */
	if(status == FILENOTEXIST)
	{
	    printf("%s \033[0;31mdoes not exist\n", filenames[i]);
	}
	else if(status == FILE_EMPTY)
	{
	    printf("%s \033[0;31mis an empty file\n", filenames[i]);
	}
	else if(status == SUCCESS)
	{
	    /* Stores the filename into the list */
	    status = store_filenames_to_list(filenames[i], f_head);
	    // at above statement change the filename array to the one where the filenames are to be stored.
	    if(status == REPEATATION)
	    {
		printf("%s \033[0;31mfile is repeated so we are not adding this file into the data base\n", filenames[i]);
	    }
	    else
	    {
		printf("%s \033[0;32m validated successfully\n", filenames[i]);
	    }
	}
	/* increment the index to point to the next command line argument  */
	i++;
    }
}

/* Check and returns the file status */
int IsFileValid(const char *fname)
{
    /* To store the status that the files was is present and non empty
       and will be used in the update function */
    extern int validate_status;
    /* Take a file pointer to store the file address */
    FILE *fptr;
    /* checks if the file exist or not */
    if((fptr = fopen(fname, "r")))
    {
	/* set the file pointer at end position */
	fseek(fptr, 0, SEEK_END);

	// If doesnt work put ftell((fptr) != 0);
	/* file pointer zero means file is empty */
	if(ftell(fptr))
	{
	    fclose(fptr);
	    return SUCCESS;
	}
	else
	{
	    validate_status = 1;
	    return FILE_EMPTY;
	}
    }
    else
    {
	validate_status = 1;
	return FILENOTEXIST;
    }
}


/* If the file is found to be valid then store it into the file list */
int store_filenames_to_list(char *f_name, Flist **head)
{
    /* To use in the update funtion */
    extern int validate_status;
    // create new node to store file name.
    Flist *new = malloc(sizeof(Flist));

    // validate node is created or not.
    if(new == NULL)
    {
	return FAILURE;
    }
    // if node created initialize its link to NULL.
    new -> link = NULL;
    // and store filename into this node.
    strcpy(new -> filename, f_name);
    // when list is empty, store this filename as first node.
    if(*head == NULL)
    {
	*head = new;
	return SUCCESS;
    }

    // local pointer for traversing the filename list.
    Flist *temp = *head;

    // traverse till the last node.
    while(temp -> link != NULL)
    {
	// if the current node has the filename free the new node and return status as REPEATED.
	if(strcmp(temp -> filename, f_name) == 0)
	{
	    /* if the file is already present then free the created node */
	    free(new);
	    validate_status = 1;
	    return REPEATATION;
	}
	// point to next node.
	temp = temp -> link;
    }
    // this last node was not check by while
    // so check it for repeatation, if repeated filename then free new node.
    if(strcmp(f_name, temp -> filename) == 0)
    {
	/* if the file is already present then free the created node */
	free(new);
	validate_status = 1;
	return REPEATATION;
    }
    // this is when the file is not repeated and temp is pointing to last node.
    // so establish the link between the last node and the new node.
    else
    {
	/* If the files is validated successfully then it will be linked with the files list */
	temp -> link = new;
    }
}

