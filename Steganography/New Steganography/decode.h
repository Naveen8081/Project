#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
typedef struct _DecodeInfo
{
    char* extn_fname;//To store the extension
    FILE *fptr_image;//To open the image file
    char*src_file; //To store output image
    int file_size;  //to store extn size
    int secret_file_size;//to store extn file size
} decodeInfo;


/* decoding function prototype */

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], decodeInfo *deInfo);

/* Perform the decoding */
Status do_decoding(decodeInfo *deInfo);

/* Open image file*/
Status open_image_file(decodeInfo *deInfo);

/* To skip the header file*/
Status skip_bmp_header(decodeInfo*deInfo);

/*Decode the magic string*/
Status decode_magic_string(decodeInfo*deInfo);

/*data to image*/
Status decode_data_from_img(char*data,int size,FILE*fptr_image);

/* decode a byte into LSB of image data array */
char decode_byte_from_lsb(char *image_buffer);

/*decode a extension size*/
Status decode_file_ext_size(decodeInfo*deInfo);

/*decode a size into LSB of image data array*/
int decode_size_from_lsb(char*image_buffer);

/*decode a file extension */
Status decode_file_ext(decodeInfo*deInfo);

/*decode a extension size*/
Status decode_file_size(decodeInfo*deInfo);

/*decode a secret data*/
Status decode_file_data(decodeInfo*deInfo);



#endif
