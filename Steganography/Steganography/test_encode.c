/*
Name:P.Naveen
Date:31-12-2024
Project Name:LSB IMAGE STEGANOGRAPHY
*/
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include <unistd.h>
#include "decode.h"
int main(int argc, char *argv[]) 
{
    if (argc < 3) //checking arguments is passed or not 
    {
        printf("Enter valid arguments!\n");
        return e_failure;
    }
    if(argc>3 && check_operation_type(argv)==e_encode) //check the operation type 
    {
        // Encoding operation
        EncodeInfo encInfo;
        if (argc <= 5)
        {
        if(read_and_validate_encode_args(argv, &encInfo) == e_success)//validating arguments 
        {
            usleep(1000000);
            printf("Read and validate is successful\n");
            do_encoding(&encInfo);
        } 
        else //if it not return e failure
        {
            printf("Read and validate is failed.\n");
            return e_failure;
        }
        }
        else
        {
            printf("Please enter the valid extension\n");
            return e_failure;
        }
        
    }
    else if(argc>=2 && check_operation_type(argv)==e_decode) //check the operation type 
    {
        // Decoding operation
        decodeInfo deInfo;
        usleep(1000000);
        printf("<--yes you are Selecting decoding-->\n");
        if (argc<=4 && read_and_validate_decode_args(argv, &deInfo) == e_success) //validating arguments
        {
            usleep(1000000);
            printf("Read and validate is successful\n");
            do_decoding(&deInfo);
        } 
        else //if it not return e failure
        {
            printf("Read and validate is failed.\n");
            return e_failure;
        }
    } 
    else //if it is return not encode either decode
    {
        printf("Invalid operation type!\n");
        return e_failure;
    }

    return e_success;
}
OperationType check_operation_type(char *argv[])
{
    if(!strcmp(argv[1],"-e"))//if it is encode 
    {
        usleep(1000000);
        printf("<--yes you are Selecting encoding-->\n");
        return e_encode;
    }
    else if(!strcmp(argv[1],"-d"))//if it is decode
    {
        return e_decode;
    }
    else//if it is return not encode either decode
    {
        printf("<--You selecting wrong option-->\n");
        return e_unsupported;
	}
}
