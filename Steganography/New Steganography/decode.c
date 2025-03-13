#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "types.h"
#include "common.h"
#include "decode.h"
#include <stdlib.h>
Status read_and_validate_decode_args(char *argv[], decodeInfo *decInfo)
{
    char *index = strstr(argv[2], ".");
    if (index == NULL)
        return e_failure;
    if (strcmp(index, ".bmp"))
        return e_failure;
    if (!strcmp(index, ".bmp"))
    {
        usleep(1000000);
        printf("yes,its bmp file\n");
        decInfo->src_file = argv[2];
    }
    else
        return e_failure;
    if (argv[3] != NULL) // check the third argument is extendion or not
    {
        char *ind = (strstr(argv[3], ".")); // check the third arguments is ".txt" or not
        if (ind == NULL)
            return e_failure;
        if (!strcmp(ind, ".txt"))
        {
            decInfo->extn_fname = strtok(argv[3], ".");
            printf("%s\n", decInfo->extn_fname);
            return e_success;
        }
        else if (!strcmp(ind, ".c"))
        {
            decInfo->extn_fname = strtok(argv[3], ".");
            printf("%s\n", decInfo->extn_fname);
            return e_success;
        }
        else if (!strcmp(ind, ".sh"))
        {
            decInfo->extn_fname = strtok(argv[3], ".");
            printf("%s\n", decInfo->extn_fname);
            return e_success;
        }
        else
        {
            printf("Enter a valid extension\n");
            return e_failure;
        }
    }
    else
    {
        decInfo->extn_fname = "output_secret";
        usleep(1000000);
        printf("Defaulty create file %s\n", decInfo->extn_fname);
    }
    return e_success;
}
Status open_image_file(decodeInfo *deInfo)
{
    deInfo->fptr_image = fopen(deInfo->src_file, "r");
    printf("%s\n", deInfo->src_file); // It is source image
    if (deInfo->fptr_image == NULL)   // check the file is open or not
    {
        perror("Error: Unable to opening file\n");
        return e_failure;
    }
    // printf("%s\n", deInfo->src_file);
    return e_success;
}
Status skip_bmp_header(decodeInfo *deInfo)
{
    fseek(deInfo->fptr_image, 54, SEEK_SET);
    int pos = ftell(deInfo->fptr_image); // skipping bmp header
    printf("Header size: %d\n", pos);
    return e_success;
}
Status decode_magic_string(decodeInfo *deInfo)
{
    int size = strlen(MAGIC_STRING);
    char decode_mag_str[size + 1]; // for storing null we allocate one more memory
    decode_data_from_img(decode_mag_str, size, deInfo->fptr_image);
    printf("Magic string: %s\n", decode_mag_str);
    if (!strcmp(decode_mag_str, MAGIC_STRING)) // check the decoded magic string matched or not
    {
        printf("Magic string is matched\n");
        return e_success;
    }
    else
    {
        printf("Magic string is not matched\n");
        return e_failure;
    }
}
Status decode_data_from_img(char *decode_mag_str, int size, FILE *fptr_image)
{
    int index = 0;
    char image_buffer[8];
    for (int i = 0; i < size; i++)
    {
        fread(image_buffer, sizeof(char), 8, fptr_image);
        decode_mag_str[index++] = decode_byte_from_lsb(image_buffer); // we are storing decoded magic string in an array
    }
    decode_mag_str[index] = '\0'; // we storing null at last index
    return e_success;
}
char decode_byte_from_lsb(char *image_buffer)
{
    char data = 0;
    for (int i = 0; i < 8; i++)
    {
        int getbits = (image_buffer[i] & 1);
        data = data | (getbits << (7 - i));
        // printf("Decoded bytes from int lsb %d\n",getbits);
        // printf("Decoded bytes from char lsb %c\n",data);
    }
    return data;
}
Status decode_file_ext_size(decodeInfo *deInfo)
{
    char buffer[32];
    fread(buffer, sizeof(char), 32, deInfo->fptr_image);
    deInfo->file_size = decode_size_from_lsb(buffer);  // storing secret file extn size in an structure
    printf("Extension Size: %d\n", deInfo->file_size); // extension size
    return e_success;
}
int decode_size_from_lsb(char *image_buffer)
{
    int data = 0;
    for (int i = 0; i < 32; i++)
    {
        int getbits = (image_buffer[i] & 1);
        data |= getbits << (31 - i);
    }
    // printf("Decoded size from LSB: %d\n", data);
    return data;
}

Status decode_file_ext(decodeInfo *deInfo)
{
    char *decode_ext = (char *)malloc(deInfo->file_size + 1); // allocating memory etc is store the null index and typecasting at char*
    if (!decode_ext) //memory allocation is fail
        return e_failure;
    decode_data_from_img(decode_ext, deInfo->file_size, deInfo->fptr_image);
    decode_ext[deInfo->file_size] = '\0';//storing the null characer at in last index
    printf("Decoded extension: %s\n", decode_ext); //find the extension
    char *new_filename = (char *)malloc(strlen(deInfo->extn_fname) + deInfo->file_size + 1);
    strcpy(new_filename, deInfo->extn_fname);
    strcat(new_filename, decode_ext);
    printf("After concenation: %s\n",new_filename);
    return e_success;
}
Status decode_file_size(decodeInfo *deInfo)
{
    char buffer[32];
    fread(buffer, sizeof(char), 32, deInfo->fptr_image);
    deInfo->secret_file_size = decode_size_from_lsb(buffer); // decoding and storing secret file size in an one variable
    printf("Secret file size: %d\n", deInfo->secret_file_size);
    return e_success;
}
Status decode_file_data(decodeInfo *deInfo)
{
    FILE *fptr_secret = fopen(deInfo->extn_fname, "w"); // we write the decode data so we open file in write mode
    if (fptr_secret == NULL)
    {
        perror("Error opening output file");
        return e_failure;
    }
    for (int i = 0; i < deInfo->secret_file_size; i++)
    {
        char image_buffer[8];
        fread(image_buffer, sizeof(char), 8, deInfo->fptr_image);
        char decoded_byte = decode_byte_from_lsb(image_buffer);
        fwrite(&decoded_byte, sizeof(char), 1, fptr_secret); // we writing one one byte into secret file
    }
    return e_success;
}
Status do_decoding(decodeInfo *deInfo)
{
    if (open_image_file(deInfo) == e_success)
    {
        usleep(1000000);
        printf("Open image file is done\n");

        if (skip_bmp_header(deInfo) == e_success)
        {
            usleep(1000000);
            printf("Header is done\n");
        }
        if (decode_magic_string(deInfo) == e_success)
        {
            usleep(1000000);
            printf("Magic string is decoded successfully\n");
            printf("OFFSET: %ld\n", ftell(deInfo->fptr_image));
        }
        if (decode_file_ext_size(deInfo) == e_success)
        {
            usleep(1000000);
            printf("extension size decoded successfully\n");
            printf("OFFSET: %ld\n", ftell(deInfo->fptr_image));
        }
        if (decode_file_ext(deInfo) == e_success)
        {
            usleep(1000000);
            printf("File extension is done\n");
            printf("OFFSET: %ld\n", ftell(deInfo->fptr_image));
        }
        if (decode_file_size(deInfo) == e_success)
        {
            usleep(1000000);
            printf("File extension size is  done\n");
            printf("OFFSET: %ld\n", ftell(deInfo->fptr_image));
        }
        if (decode_file_data(deInfo) == e_success)
        {
            usleep(1000000);
            printf("File data decoded is done\n");
            printf("OFFSET: %ld\n", ftell(deInfo->fptr_image));
        }
        usleep(1000000);
        printf("<--Decoding is successfully completed-->\n");
    }
    else
    {
        usleep(1000000);
        printf("<--Decoding is not completed-->\n");
    }
}
