/*
Name : Sandesh.
Date : 04/05/2022
Description :	
Sample input :	
Sample output :	
 */

/* Imp the size of the tag is find from the bits combination of the SIZE bits which are in four consecutive bytes following the TAG,
   This is done by storing all those size bytes into a character array and then reversing that array as bytes were arranged in the BIG ENDIAN format,
   and now making an integer pointer to point the first character this way it will give the size in integer form,
   we can do it vice versa i.e. store these bits in the in the int variable by typecasting to char and then reading that int variable */

#include"metadata.h"
char tag[5];
char *tag_name[6] = {"Title", "Artist", "Album", "Year", "Music", "Comment"};
char *tag_code[6] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "TCOM"};
char options[6] = {'t', 'a', 'A', 'm', 'y', 'c'};
char *title, *artist, tag_size[5];
int main(int argc, char *argv[])
{
    char *extn;
    FILE *file;

    if(argc >= 2)
    {
	/* Check that the valide option is passed and proceed */
	if(strlen(argv[1]) == 2 && strcmp(argv[1], "-v") == 0)
	{
	    if(extn = strchr(argv[2], '.'))
	    {
		if(strcmp(extn, ".mp3") == 0)
		{
		    printf("\n\033[0;33mExtension found as: %s\n", extn);
		    /* open file in read only mode */
		    file = fopen(argv[2], "r");
		    tag[4] = '\0';
		    // For Debuge purpose:- printf("file position at: %ld\n", ftell(file));
		    /* Scan the first three characters of the header as they will contain the version name */
		    fread(tag, 3, 1, file);
		    tag[3] = '\0';
		    /* Validate that the version name is ID3 */
		    printf("\033[0;33mMetadata container found as :%s\n", tag);
		    if(strcmp(tag, "ID3") == 0)
		    {
			/* Read the version info
			   fread(tag, 4, 1, file);
			   tag[5] = '\0';
			   display_tag_data(tag, 4);
			 */
			view(file);
		    }
		    else
		    {
			printf("\n\033[0;31mError: Its not an ID3 meta data container\n\n");
			error_message(argv[0]);
		    }
		}
		else
		{
		    printf("\n\033[0;31m.mp3 extension not found\n\n");
		    error_message(argv[0]);
		}
	    }
	    else
	    {
		printf("\n\033[0;31mNo extension exist\n\n");
		error_message(argv[0]);
	    }
	}
	else if(strlen(argv[1]) == 2 && strcmp(argv[1], "-e") == 0)
	{
	    /* For Debuge purpose : printf("Inside the editor\n");
	    /* Check that the valide options are passed and proceed */
	    /* open file in write mode */
	    if(extn = strchr(argv[4], '.'))
	    {
		if(strcmp(extn, ".mp3") == 0)
		{
		    printf("\n\033[0;33mExtension found as: %s\n", extn);
		    /* open file in read only mode */
		    file = fopen(argv[4], "r+");
		    tag[4] = '\0';
		    // For Debuge purpose:- printf("file position at: %ld\n", ftell(file));
		    /* Scan the first three characters of the header as they will contain the version name */
		    fread(tag, 3, 1, file);
		    tag[3] = '\0';
		    /* Validate that the version name is ID3 */
		    printf("\033[0;33mMetadata container found as :%s\n", tag);
		    if(strcmp(tag, "ID3") == 0)
		    {
			/* Read the version info
			   fread(tag, 4, 1, file);
			   tag[5] = '\0';
			   display_tag_data(tag, 4);
			 */
			char *temp_tag;
			if(argc >= 5)
			{
			    if((temp_tag = fetch_tag_code(argv[2][1])) != NULL)
			    {
				edit(argv[3],temp_tag, file);

			    }
			    else
			    {
				printf("\033[0;31mError: Required tag is not available in the file\n");
			    }
			}
			else
			{
			    printf("\n\033[0;31mError: Please pass sufficient number of arguments\n\n");
			    error_message(argv[0]);

			}
		    }
		    else
		    {
			printf("\n\033[0;31mError: Its not an ID3 meta data container\n\n");
			error_message(argv[0]);
		    }
		}
		else
		{
		    printf("\n\033[0;31mError: .mp3 extension not found\n\n");
		    error_message(argv[0]);
		}
	    }
	    else
	    {
		printf("\n\033[0;31mError: No extension exist\n\n");
		error_message(argv[0]);
	    }
	}
	else if(strlen(argv[1]) == 6 && strcmp(argv[1], "--help") == 0)
	{
	    printf("\033[0;34m1. -v -> veiw mp3 file contents\n2. -e -> edit mp3 file contents\n\t2.1. -t -> to edit song title\n\t2.2. -a -> to edit artist name\n\t2.3. -A -> to edit year\n\t2.5. -m -> to edit content\n\t2.1. -c -> to edit comment\n");

	}
	else
	{
	    printf("\n\033[0;31mError: Passed option(s) is/are not valid\n\n");
	    error_message(argv[0]);
	}
    }
    else
    {
	printf("\n\033[0;31mError: Insufficient arguments passed\n\n");
	error_message(argv[0]);
    }
    return 0;
}

Status error_message(char *file_name)
{
    printf("\033[0;37m-------------------------------------------------------------------------------------------------------------\n\n\033[0;31mError: %s : INVALID ARGUMENTS\n\033[0;33mUSAGE :\nTo view please pass like: %s -v mp3filename.mp3\nTo edit please pass like : %s -e -t/-a/-A/-m/-y/-c changing_text mp3filename.mp3\nTo get help pass like : %s --help\n\033[0;37m-------------------------------------------------------------------------------------------------------------\n", file_name, file_name, file_name, file_name);
}


char *fetch_tag_code(char ch)
{
    switch(ch)
    {
	case 't':
	    return "TIT2";
	    break;
	case 'a':
	    return "TPE1";
	    break;
	case 'A':
	    return "TALB";
	    break;
	case 'y':
	    return "TYER";
	    break;
	case 'm':
	    return "TCON";
	    break;
	case 'c':
	    return "TCOM";
	    break;
	default:
	    return NULL;
    }
}



Status rev_str(char *str)
{
    for(int i = 0; i <= 1; i++)
    {
	char temp = str[i];
	str[i] = str[3-i];
	str[3-i] = temp;
    }
    return success;
}

Status display_tag_data(char *str, int size)
{
    for(int i = 0; i < size; i++)
    {
	printf("%c", str[i]);
    }
    return success;
}

Status view(FILE *file)
{
    char *tag_ptr;
    int *size = (int *) tag_size;
    printf("\033[0;32m-------------------------------------SELECTED VIEW DETAILS---------------------------------------------------\n\n\033[0;36m-------------------------------------------------------------------------------------------------------------\n				\033[0;32mMP3 TAG READER AND EDITOR FOR ID3v2				\n\033[0;36m-------------------------------------------------------------------------------------------------------------");
    ;
    // printf("file position at: %ld\n", ftell(file));
    fseek(file, 10, SEEK_SET);
    for(int i = 0; i < 6; i++)
    {

	fread(tag, 4, 1, file);
	//printf("\nTag found as:%s\n", tag);
	tag[4] = '\0';
	/* Here tag_name being used to store next 4 bytes giving size of the tag name in interger */
	fread(tag_size, 4, 1, file);  // I am not able to read the proper size here.
	rev_str(tag_size);
	//printf("Size of the tag:%d\n", *size);

	tag_ptr = malloc(sizeof(char) * (*size));
	// title[*size] = '\0';
	/* extra one for avoiding the first non printable character in the tag data */
	fseek(file, ftell(file) + 2 + 1, SEEK_SET);
	/* -1 as we have already added a one above, this will make sure that the file position stops at the first character of the next tag */
	fread(tag_ptr, *size - 1, 1, file);
	//printf("tag data is:");
	printf("\n%15s\t\t:\t", tag_name[i]);
	display_tag_data(tag_ptr, *size);
    }
    printf("\n\033[0;36m-------------------------------------------------------------------------------------------------------------\n\n\033[0;32m----------------------------------DETAILS DISPLAYED SUCCESSFULLY---------------------------------------------\n");
}

Status edit(char *argv, char *temp_tag, FILE *file)
{
    char *tag_ptr;
    int *size = (int *) tag_size;
    fseek(file, 10, SEEK_SET);
    for(int i = 0; i < 6; i++)
    {
	fread(tag, 4, 1, file);
	tag[4] = '\0';
	if(strcmp(tag, temp_tag) == 0)
	{
	    fread(tag_size, 4, 1, file);  
	    rev_str(tag_size);
	    fseek(file, ftell(file) + 2 + 1, SEEK_SET);
	    printf("%ld\n", ftell(file));
	    fwrite(argv, *size - 1, 1, file);
	    return success;
	}

	fread(tag_size, 4, 1, file);  
	rev_str(tag_size);

	tag_ptr = malloc(sizeof(char) * (*size));
	fseek(file, ftell(file) + 2 + *size, SEEK_SET);
    }

}



