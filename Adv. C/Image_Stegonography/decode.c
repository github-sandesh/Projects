
// include all the required header files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "decode.h"
#include "types.h"
#include "unistd.h"

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_d_files(DecodeInfo *decInfo)
{
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "\033[0;31mERROR: Unable to open file %s\n", decInfo->stego_image_fname);

	return e_failure;
    }

    // Secret file 
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    // Do Error handling 
    if (decInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "\033[0;31mERROR: Unable to open file %s\n", decInfo->secret_fname);
	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
// this is where the command line arguments will be checked and validated.
Status read_and_validate_decode_args( char *argv[], DecodeInfo *decInfo )
{
    // validate the .bmp image file.
    if ( strcmp(strchr(argv[2], '.'), ".bmp") == 0 )
    {
	decInfo->stego_image_fname = argv[2];
    }
    else
	return e_failure;

    // validate the .txt file.
    if ( argv[3] != NULL )
    {
	decInfo -> secret_fname = argv[3];
    }
    else
    {
	decInfo -> secret_fname = "decoded.txt";
    }
    return e_success;
}
// this function meant to extract the magic string data from the stego image.
Status decode_magic_string( FILE *fptr_stego_image, DecodeInfo *decInfo)
{
    // put the stego image file pointer to the position pointing to the 54th byte of the file from where we will scan the following data.
    fseek(decInfo -> fptr_stego_image, 54, SEEK_SET);
    uint magic_str_len = strlen(MAGIC_STRING);
    printf("\n\033[0;36mno of magic characters: %d\n", magic_str_len);
    char *str = malloc(magic_str_len);
    // decodes two bytes of the stego image to know the magic string.
    for (uint i = 0; i < magic_str_len; i++ )
    {
	decode_byte_from_lsb(decInfo -> fptr_stego_image, decInfo);
	str[i] = decInfo -> byte_from_lsb;
    }
    printf("\033[0;36mDecoded magic string is : %s\n", str);
    // if the desired magic string is present then return success else failure.
    if ( strcmp ( str, MAGIC_STRING ) == 0 )
    {
	free(str);
	return e_success;
    }
    else
    {
	free(str);
	return e_failure;
    }

}
// this function will decode the lsb of the stego image byte and form the original byte data from it.
Status decode_byte_from_lsb( FILE *fptr_stego_image, DecodeInfo *decInfo)
{
    char str[32];
    char byte = 0;
    fread( str, 8, 1,decInfo -> fptr_stego_image);
    // one by one scan all the bytes lsb of the stego image and decode original byte from 8 bytes of it.
    for( int i = 0; i < 8; i++ )
    {
	if ( str[i] & 0x01 )
	{
	    byte = (byte << 1) | 0x01;
	}
	else
	{
	    byte = (byte << 1) | 0x00;
	}
    }
    // store that decoded byte into the structure pointer.
    decInfo -> byte_from_lsb = byte;
    return e_success;

}
// this is meant to decode the extension size from the
Status decode_extn_size(int ext_size, DecodeInfo *decInfo)
{
    int byte = 0;
    printf("\n\033[0;36mDesired extension size is : %d\n",ext_size);
    fseek(decInfo -> fptr_stego_image, 54+strlen(MAGIC_STRING)*8, SEEK_SET);
    char str[32*8];
    fread( str, 32, 1,decInfo -> fptr_stego_image);
    // takes one by one byte of the stego image and form the one byte from lsb of 8 bytes of stego image.
    // and store those bytes one by one into the int variable.
    for( int i = 0; i < 32; i++ )
    {
	byte = (byte << 1) | (str[i] & 0x01);
    }
    printf("\033[0;36mDecoded extension size is : %d\n", byte);
    // now if that integer variable gives the value i.e. 4 then proceed else return error.
    if ( ext_size == byte )
	return e_success;
    else
	return e_failure;
}
// this function meant to extract the extension of the original file.
Status decode_secret_file_extn( char * file_extn, DecodeInfo *decInfo)
{
    printf("\n\033[0;36mRequired file extension is : %s\n", file_extn);
    fseek(decInfo -> fptr_stego_image, 54, SEEK_SET);
    uint extn_size = strlen(file_extn);
    fseek(decInfo -> fptr_stego_image, 54+(strlen(MAGIC_STRING)+sizeof(int))*8, SEEK_SET);
    char *extn = malloc(extn_size);
    // one by one decode the original byte from the stego image
    // and store in the dynamic memory.
    for (int i = 0; i < extn_size; i++ )
    {
	decode_byte_from_lsb(decInfo -> fptr_stego_image, decInfo);
	extn[i] = decInfo -> byte_from_lsb;
    }
    printf("\033[0;36mDecoded file extension is : %s\n", extn);
    // now if this data is same as the .txt then okay else return error.
    if(strcmp(extn, file_extn) == 0)
    {
	free(extn);
	return e_success;
    }
    else
    {
	free(extn);
	return e_failure;
    }
}
// this will decode the secret file size from the stego image.
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    fseek(decInfo -> fptr_stego_image, 54+(strlen(MAGIC_STRING)+sizeof(int)+strlen(decInfo -> extn_secret_file))*8, SEEK_SET);
    int byte = 0;
    char str[32*8];
    fread( str, 32, 1,decInfo -> fptr_stego_image);
    // take one by one stego image byte and store its lsb bit into the byte variable for all the proceding stego image bytes.
    for( int i = 0; i < 32; i++ )
    {
	byte = (byte << 1) | (str[i] & 0x01);
    }
    printf("\n\033[0;36mDecoded secret file size is : %d\n", byte);
    // store this decoded secret file size into the structure member.
    decInfo -> size_secret_file = byte;
    return e_success;
}
// decode secret data from the file and store another text file.
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    printf("\033[0;36mDecoded data is : \n");
    fseek(decInfo -> fptr_stego_image, 54+(strlen(MAGIC_STRING)+sizeof(int)*2+strlen(decInfo -> extn_secret_file))*8, SEEK_SET);
    char ch[1];
    // one by one scan the lsb of stego image and form the original byte from it
    // and store that byte into the text file.
    for (int i = 0; i < decInfo -> size_secret_file; i++ )
    {
	decode_byte_from_lsb(decInfo -> fptr_stego_image, decInfo);
	ch[0] = decInfo -> byte_from_lsb;
	// writes each byte into the files one by one.
	fwrite(ch, 1, 1, decInfo -> fptr_secret);
	printf("%c", ch[0]);
    }
    printf("\n");
    return e_success;
}

// this function has to do the decoding work specially.
Status do_decoding(DecodeInfo *decInfo)
{
    // this will open the files required for the decoding.
    if( open_d_files(decInfo) == e_success)
    {
	printf("\033[0;32mOpening files is a success\n");
	// decoded magic string, if fails then prints the error message.
	if(decode_magic_string(decInfo -> fptr_stego_image, decInfo) == e_success)
	{
	    printf("\033[0;32mmagic string decoded successfully\n");
	    // store the target file extension into the structure member.
	    strcpy(decInfo -> extn_secret_file , strstr(decInfo -> secret_fname , "." ));
	    // decode the extension from the stego fiel.
	    if(decode_extn_size( strlen(decInfo -> extn_secret_file), decInfo) == e_success)
	    {
		printf("\033[0;32mDecoding extension size is successfull\n");
		// decodes the extension from the stego file.
		if(decode_secret_file_extn( decInfo -> extn_secret_file, decInfo) == e_success )
		{
		    printf("\033[0;32mSecret file extension decoded successfully\n");
		    // decodes the secret file size from stego image.
		    if(decode_secret_file_size(decInfo) == e_success)
		    {
			printf("\033[0;32mDecoding secret file size is successfull\n");
			// decodes the secret file data from the stego image
			// and stores it into the text file.
			if(decode_secret_file_data(decInfo) == e_success)
			{
			    printf("\033[0;32mSecret data decoded successfully\n");
			}
			else
			{
			    printf("\033[0;31mFailed to decode the secret data\n");
			}

		    }
		    else
		    {
			printf("\033[0;31mFailed to decode secret file size\n");
			return -1;
		    }

		}
		else
		{
		    printf("\033[0;31mDecoding secret file extension failed\n");
		    return -1;
		}
	    }
	    else
	    {
		printf("\033[0;31mDecoding extension size failed\n");
		return -1;
	    }
	}
	else
	{
	    printf("\033[0;31merror while decoding the magic string\n");
	    return -1;
	}
    }
    else
    {
	printf("\033[0;31mOpen files is a failure\n");
	return e_failure;
    }

    return e_success;
}


