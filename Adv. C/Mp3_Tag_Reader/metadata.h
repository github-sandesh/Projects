#ifndef METADATA_H
#define METADATA_H
/* Including the standard header files */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef enum
{
    success,
    failure
} Status;

Status rev_str(char *str);
Status display_tag_data(char *str, int size);
Status view(FILE *file);
Status error_message(char *file_name);
char *fetch_tag_code(char ch);
Status edit(char *argv,char *temp_tag, FILE *file);

#endif  /* End of the MAIN_H */
