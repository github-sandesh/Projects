/*      Documentation
	Name : Sandesh.
	Date : 12/06/2022
 Description : Project_1: Steganography.
Sample input :  1) for encoding use following.
		./a.out -e beautiful.bmp secret.txt

		2) for decoding use following.
		./a.out -d stego.bmp

Sample output : 1) for encoding output is as follows.
		
		Selected encoding
		Read and validate encode arguments is a success
		---------------Started Encoding----------
		Open files is a success
		width = 1024
		height = 768
		Check capacity is a success
		Copy bmp header is a success
		Encoded Magic string
		Extn is .txt
		Encoded size of extn
		Encoded extn successfully
		Encoded secret file size
		Encoded secret file data
		Copied remaining data sccessfully
		------------------Encoded successfully-------------------

		2) for decoding output is as follows.

		Selected decoding
		Read and validate encode arguments is a success
		----------------------Started Decoding--------------------
		Opening files is a success

		no of magic characters: 2
		Decoded magic string is : #*
		magic string decoded successfully

		Desired extension size is : 4
		Decoded extension size is : 4
		Decoding extension size is successfull

		Required file extension is : .txt
		Decoded file extension is : .txt
		Secret file extension decoded successfully

		Decoded secret file size is : 25
		Decoding secret file size is successfull
		Decoded data is :
		My password is SECRET ;)

		Secret data decoded successfully
		-------------------Decoding completed-------------------
*/


// including all the required header files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main( int argc, char *argv[] )
{
    // validate that more than three argumets are to be passed.
    if(argc >= 3)
    {
	// this will be executed if the -e option passed through command line.
	if ( check_operation_type( argv ) == e_encode )
	{
	    printf("\033[0;32mSelected encoding\n");
	    // declare the structure variable typedef to EncodInfo as encInfo.
	    EncodeInfo encInfo;
	    // validate that required argumets are passed.
	    if(read_and_validate_encode_args(argv, &encInfo) == e_success )
	    {
		printf("\033[0;32mRead and validate encode arguments is a success\n");
		printf("\033[0;33m---------------Started Encoding----------\n");
		// call this function to do decoding.
		if ( do_encoding(&encInfo) == e_success )
		{
		    printf("\033[0;32m------------------Encoded successfully-------------------\n");
		}
		else
		{
		    printf("\033[0;31m----------Failed to encode----------\n");
		    return -1;
		}
	    }
	    else
	    {
		printf("\033[0;31mRead and validate encode arguments is a failure\n");
		return -1;
	    }

	}

	// if -d option is passed through the command line then execute this.
	else if( check_operation_type( argv ) == e_decode )
	{
	    printf("\033[0;33mSelected decoding\n" );
	    // declare the structure variable as decInfo which is typedef to DecodeInfo.
	    DecodeInfo decInfo;
	    // this will check whether the valid arguments are passed aor not.
	    if(read_and_validate_decode_args(argv, &decInfo) == e_success )
	    {
		printf("\033[0;32mRead and validate encode arguments is a success\n");
		printf("\033[0;33m----------------------Started Decoding--------------------\n");
		// this will call all the decoding related functions.
		if ( do_decoding(&decInfo) == e_success )
		{
		    printf("\033[0;32m--------------Decoding successfully completed---------------\n");
		}	
		else
		{
		    printf("\033[0;31m---------------Failed to decode----------------\n");
		    return -1;
		}
	    }
	    else
	    {
		printf("\033[0;31mRead and validate decode arguments is a failure\n");
		return -1;
	    }

	}
	else
	{
	    // if invalid arguments are passed then print these messages.
	    printf("\033[0;31mInvalid input\n\033[0;33mfor Encoding: .a.out -e beautiful.bmp secret.txt [stego.bmp]\nFor decoding: .a.out -d stego.bmp [ decode.txt]\n");
	}
    }
    else
    {
	// if less than three no of arguments are passed then print this message.
	printf("\033[0;31mError : Kindly pass for\nEncoding: 4 arguments\nDecoding: 3 arguments\nInvalid input\n\033[0;33mfor Encoding: .a.out -e beautiful.bmp secret.txt [stego.bmp]\nFor decoding: .a.out -d stego.bmp [decode.txt] \n");
    }


    return 0;
}
// this function is meant to find whether option passed is -d or -e and return the enum variable accordingly.
OperationType check_operation_type( char *argv[] )
{
    if (strcmp( argv[1], "-e") == 0 )
	return e_encode;
    else if (strcmp( argv[1], "-d") == 0 )
	return e_decode;
    else
	return e_unsupported;
}

