#ifndef APC_H
#define APC_H
#define SUCCESS 0
#define FAILURE -1

#define SIZE_ALLOC_EACH_TIME 4

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdio_ext.h>
#include<ctype.h>


typedef unsigned int uint;
typedef int data_t;

typedef enum
{
    success,
    failure
} Status_t;

typedef struct node
{
	struct node *prev;
	data_t value;
	struct node *next;
}Dlist_t;

/* Include the prototypes here */

/* To validate the command line arguments */
//int validate_cla(int argc);
Status_t create_list(Dlist_t **head, Dlist_t **tail, char *str, int start, int end);
Status_t store_expression(char **str, uint *str_end, uint *oper_index);
int insert_at_first(Dlist_t **head, Dlist_t **tail, int value);
int print_list(Dlist_t *head);
int delete_list(Dlist_t **head, Dlist_t **tail);
int addition(Dlist_t *head1, Dlist_t *tail1, Dlist_t *head2, Dlist_t *tail2, Dlist_t **headR, Dlist_t ** tailR);
int subtraction(Dlist_t *head1, Dlist_t *tail1, Dlist_t *head2, Dlist_t *tail2, Dlist_t **headR, Dlist_t ** tailR);
#endif
