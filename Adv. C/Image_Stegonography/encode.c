// including all the necessary header files.
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "encode.h"
#include "types.h"

/* Function Definitions */
/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("\033[0;33mwidth = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("\033[0;33mheight = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "\033[0;31mERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "\033[0;31mERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "\033[0;31mERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
// here the argumets are check whether they are as per the requirement.
Status read_and_validate_encode_args( char *argv[], EncodeInfo *encInfo )
{
    // if .bmp file is not passed then print the error.
    if ( strcmp(strchr(argv[2], '.'), ".bmp") == 0 )
    {
	encInfo->src_image_fname = argv[2];
    }
    else
	return e_failure;

    // if .txt file is not passed then print the error.
    if(argv[3] == NULL)
    {
	printf("\033[0;31mError : Kindly pass for\nEncoding: 4 arguments\nDecoding: 3 arguments\n\033[0;34mUsage: ./a.out -e beautiful.bmp secret.txt\n");
	return e_failure;
    }
    else
    {
	// if the file is passed but its not .txt file then print the error.
	if(strcmp(strstr(argv[3], "."), ".txt") == 0 )
	{
	    encInfo -> secret_fname = argv[3];
	}
	else
	{
	    printf("\033[0;31mError: secret file should be .txt file");
	}
    }

    // if fourth argument is passed then give that name to the decoded image file.
    // else creat new file of the name stego.bmp.
    if ( argv[4] != NULL )
	encInfo -> stego_image_fname = argv[4];
    else
	encInfo -> stego_image_fname = "stego.bmp";
    return e_success;
}
// this function checks if the secret file data can be accomodated in the image file passed or not.
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    /* 54 to is the header space,
       16 is the #@ space,
       32 is the 4 extension characters,
       32 is for the 4 bytes to store int value of no of bytes of the secret file contents,
       32 is for the 4 bytes
     */
    if(encInfo -> image_capacity > (54 + 16 + 32 + 32 + 32 + (encInfo -> size_secret_file * 8) ))
	return e_success;
    else
	return e_success;
}
// this function finds the size of the secret file and returns that value.
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}
// this function will copy the header of the source image as it is into the targeted file.
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char str[54];
    fseek(fptr_src_image, 0, SEEK_SET);
    fread(str, 54, 1, fptr_src_image);
    fwrite(str, 54, 1, fptr_dest_image);
    return e_success;
}
// after the header we have to encode the magic string data into the stego file.
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image( magic_string, strlen(magic_string), encInfo );
    return e_success;
}
// this function encodes the passed data to the target stego image.
Status encode_data_to_image( const char *data, int size, EncodeInfo *encInfo)
{
    for(int i = 0; i < size; i++ )
    {
	fread(encInfo -> image_data, 8, 1, encInfo -> fptr_src_image);
	encode_byte_to_lsb(data[i], encInfo -> image_data);
	fwrite(encInfo -> image_data, 8, 1, encInfo -> fptr_stego_image);
    }
    return e_success;
}
// this function will encode the data present in image_buffer to the lsb of the target image.
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i = 0; i < 8; i++ )
    {
	image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (7-i)) & 01);
    }
    return e_success;
}
// this function has to encode the size of extension.
Status encode_extn_size(int extn_size, EncodeInfo *encInfo)
{
    encode_size_to_lsb(extn_size, encInfo);
    return e_success;
}
// this function will encode the secret file extension to the lsb of the target image.
Status encode_size_to_lsb(int size, EncodeInfo *encInfo)
{
    char str[32];
    fread(str, 32, 1, encInfo -> fptr_src_image);
    for(int i = 0; i < 32; i++ )
    {
	str[i] = (str[i] & 0xFE) | ((size >> (31-i)) & 01);
    }
    fwrite(str, 32, 1, encInfo -> fptr_stego_image);
}
// now encode extension of secret file to the the target stego file.
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn, strlen(file_extn), encInfo);
    return e_success;
}

// the actual size of the secret file is encoded into the stego file.
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    encode_size_to_lsb(file_size, encInfo);
    return e_success;
}
// now all the secret file data will be encoded into the stego file.
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char str[encInfo -> size_secret_file];
    fseek(encInfo -> fptr_secret, 0, SEEK_SET);
    fread(str, encInfo -> size_secret_file, 1, encInfo -> fptr_secret);
    encode_data_to_image(str, encInfo -> size_secret_file, encInfo );
    return e_success;
}
// now that all the reamaining data will be copied as it is.
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch, 1, 1, fptr_src) > 0)
    {
	fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}
// this is to do encoding specially.
Status do_encoding(EncodeInfo *encInfo)
{
    // open the files for operation.
    if( open_files(encInfo) == e_success)
    {
	printf("\033[0;32mOpen files is a success\n");
	// validate capacity of the source image is sufficient to store secret data.
	if(check_capacity(encInfo) == e_success)
	{
	    printf("\033[0;32mCheck capacity is a success\n");
	    // copy the source image header as it is.
	    if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success )
	    {
		printf("\033[0;32mCopy bmp header is a success\n");
		// the magic string is encoded into the lsb of the targeted image file.
		if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
		{
		    printf("\033[0;32mEncoded Magic string\n");
		    // store the store the extension of the secret file into the structure member.
		    strcpy(encInfo -> extn_secret_file , strstr(encInfo -> secret_fname , "." ));
		    printf("\033[0;33mSecret fiel extension is \033[0;34m%s\n", encInfo -> extn_secret_file);
		    // now encode this size of the extension into the lsb of the targeted image file.
		    if(encode_extn_size(strlen(encInfo -> extn_secret_file), encInfo) == e_success)
		    {
			printf("\033[0;32mEncoded size of extn\n");
			// now encode extension of the secret file into the target image file.
			if(encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_success)
			{
			    printf("\033[0;32mEncoded extn successfully\n");
			    // now encode the secret file size into lsb if th target image file.
			    if(encode_secret_file_size(encInfo -> size_secret_file, encInfo) == e_success)
			    {
				printf("\033[0;32mEncoded secret file size\n");
				// finally encode the secret file data into the target image file.
				if(encode_secret_file_data(encInfo) == e_success)
				{
				    printf("\033[0;32mEncoded secret file data\n");
				    // this function will copy the reamaining image data to the stego file.
				    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success )
				    {
					printf("\033[0;32mCopied remaining data successfully\n");
				    }
				    else
				    {
					printf("\033[0;31mFailed to copy remaining data\n");
					return e_failure;
				    }

				}
				else
				{
				    printf("\033[0;31mFailed to encode secret file data\n");
				    return e_failure;
				}
			    }
			    else
			    {
				printf("\033[0;31mFailed to encode file size\n");
				return e_failure;
			    }
			}
			else
			    {
				printf("\033[0;31mFailed to encode extn\n");
				return e_failure;
			    }
		    }
		    else
		    {
			printf("\033[0;31mFailed to encode size of extn");
			return e_failure;
		    }
		}
		else
		{
		    printf("\033[0;31mFailed to encode magic string\n");
		    return e_failure;
		}
	    }
	    else
	    {
		printf("\033[0;31mFailed to copy bmp header\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("\033[0;31mCheck capacity is a failure\n");
	    return e_failure;
	}
	
    }
    else
    {
	printf("\033[0;31mOpen files is a failure\n");
	return e_failure;
    }

    return e_success;
}












