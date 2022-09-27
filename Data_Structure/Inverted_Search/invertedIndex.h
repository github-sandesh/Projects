// define this if not defined elsewhere.
#ifndef INVERTED_SEARCH_H
#define INVERTED_SEARCH_H

// insludes necessary header files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// size for the filename storage.
#define FNAME_SIZE 50

// defining macros for return values.
#define FAILURE -1
#define SUCCESS 0
#define REPEATATION -4
#define FILE_EMPTY -2
#define FILENOTEXIST -3
#define NAME_LENGTH 15
#define MAX_WORD_SIZE 13
#define MAX_FILE_SIZE 50

// structure for file list
typedef char data_t;

/* node for storing the filenames */
typedef struct file_node
{
    data_t filename[FNAME_SIZE];
    struct file_node *link;

} Flist;

/************** It is necessary to write the file table before the Wlist since we need Tlink pointer in the word_node
  if we use it after the Wlist it will give error as, it is not available to the compiler untill declared before it.
 *************/
// for file table.
// to store the file names and word counts.
typedef struct linkTable_node
{
    char f_name[NAME_LENGTH];
    int w_count;
    struct linkTable_node *link;

} Tlink;

// structure for word, file count and links to the table and to next word node storage.
typedef struct word_node
{
    int f_count;
    char word[NAME_LENGTH];
    Tlink *table_link;
    struct word_node *link;

} Wlist;

/* Recieves the commandline arguments pointer and the file list pointer. */
void validate_n_store_filenames(Flist **f_head, char *filenames[]);

/* Recieves the filename pointer and the file list pointer */
int store_filenames_to_list(char *f_name, Flist **head);

/* Recieves filename pointer and checks files is present and non empty */
int IsFileValid(const char *);

/* Recieves file listpointer and wordlist pointer and stores the database into it */
int create_DB(Flist *file_head, Wlist **head);

/* Recieves the word list structer pointer and filename pointer, reads the words from file one by one */
void read_datafile(Wlist **head, char *filename);

/* Recieves head of word list and filename */
int update_word_count(Wlist **head, char *filename);

/* Recieves word list pointer and a word and update this in the word node inserted at last */
int insert_at_last_main(Wlist **head, char *word);

/* Recieves word node pointer and will update the data word count and filename */
int update_link_table(Wlist **W_node);

/* Displays the entire database, by recieving the word list pointer */
int display_DB(Wlist **);

/* Recieves the word list pointer and char pointer and searches for the given word in the database */
int search_DB(Wlist **head, char *);

/* Update the data from the given file into the data base */
int update_DB(Flist **f_head, Wlist **_HT_head, char *filename);

/* saves whole database into the given file */
int save_DB(Wlist **head, Flist *f_head, char *);

#endif
