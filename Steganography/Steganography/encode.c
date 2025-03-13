#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include"common.h"
#include<unistd.h>
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
Status check_capacity(EncodeInfo*encInfo)
{
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);//we are calculating full image size
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);//calculate the secret file size
    if(encInfo->image_capacity>(54+((strlen(MAGIC_STRING)*8)+(4*8)+(4*8)+(4*8)+(encInfo->size_secret_file*8))))
    return e_success;
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    rewind(fptr_src_image);
    fread(buffer,sizeof(char),54,fptr_src_image);
    fwrite(buffer,sizeof(char),54,fptr_dest_image);
    return e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    
    encode_data_to_image(MAGIC_STRING,strlen(MAGIC_STRING),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
    
}
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    int i;
    //printf("%d\n",size);
    char image_buffer[8];
    //printf("file offset:%ld\n",ftell(fptr_src_image));
    for(i=0;i<size;i++)
    {
        fread(image_buffer,sizeof(char),8,fptr_src_image);
        // printf("HEXA DECIMAL VALUE: %X\n",data[i]);
        // printf("Before encoding: ");
        // for(int i=0;i<8;i++)
        // {
        //     printf("%X",image_buffer[i]);
        // }
        // printf("\n");
        encode_byte_to_lsb(data[i],image_buffer);//fun call
        //printf("After encoding: ");
        fwrite(image_buffer,sizeof(char),8,fptr_stego_image);
        // for(int i=0;i<8;i++)
        // {
        //     printf("%X",image_buffer[i]);
        // }
        // printf("\n");
        
    }
    return e_success;
}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
      image_buffer[i] = (image_buffer[i] & ~1) | ((data >> (7-i))&1);//we are get the value in msb and store into the lsb
    }
    return e_success;
}
Status encode_secret_file_extn_size(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[32];
    file_extn=encInfo->extn_secret_file;//we are encode secret file extn size
    //printf("extension: %s\n",encInfo->extn_secret_file);
    fread(buffer,sizeof(char),32,encInfo->fptr_src_image);
    encode_size_to_lsb(strlen(file_extn),buffer);
    fwrite(buffer,sizeof(char),32,encInfo->fptr_stego_image);
    return e_success;
}
Status encode_size_to_lsb(int size, char *buffer)
{
    for (int i = 0; i < 32; i++) 
    {
        buffer[i] = (buffer[i] & ~1) | ((size >> (31 - i)) & 1);
    }
    return e_success;
}

Status encode_secret_file_extn(EncodeInfo *encInfo)
{
    uint size=strlen(encInfo->extn_secret_file);//we are encode secret file extn
    printf("secret file extn: %s\n",encInfo->extn_secret_file);
    encode_data_to_image(encInfo->extn_secret_file,size,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_size(long file_size,EncodeInfo *encInfo)
{
    char buffer[32];//we are encode the secret file size
    fread(buffer,sizeof(char),32,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,buffer);
    fwrite(buffer,sizeof(char),32,encInfo->fptr_stego_image);
    return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char image_data[encInfo->size_secret_file];//we are encode the secret file data
    printf("file size: %ld\n",encInfo->size_secret_file);
    rewind(encInfo->fptr_secret);
    //printf("offset: %ld\n",ftell(encInfo->fptr_secret));
    fread(image_data,sizeof(char),encInfo->size_secret_file,encInfo->fptr_secret);
    image_data[encInfo->size_secret_file]=encode_data_to_image(image_data,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    //printf("%s",image_data);
    return e_success;
}
Status copy_remaining_img_data(EncodeInfo *encInfo)
{
    char buffer;
    //we encode the image data till end of the file 
    uint remain =((encInfo->image_capacity)+54)-(ftell(encInfo->fptr_src_image)-1);
    for(int i=0;i<remain;i++)
    {
        fread(&buffer,1,1,encInfo->fptr_src_image);
        fwrite(&buffer,1,1,encInfo->fptr_stego_image);
    }
    return e_success;
}
uint get_file_size(FILE*fptr_secret)
{
    uint fp;
    fp=fseek(fptr_secret,0,SEEK_END);
    fp=ftell(fptr_secret);
    //printf("Secret file size: %d\n",fp);
    return fp;
}
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

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
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	   printf("ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}   
Status read_and_validate_encode_args(char **argv, EncodeInfo *encInfo)
{
    char*index=strstr(argv[2],".");
    if(index==NULL)
    return e_failure;
    if(strcmp(index,".bmp"))
    return e_failure;
    if(!strcmp(index,".bmp"))
    {
        usleep(1000000);
        printf("yes,its bmp file\n");
        encInfo->src_image_fname=argv[2];
    }
    else
    {
        return e_failure;
    }
    char*ind=(strstr(argv[3],".")); //check the third arguments is ".txt" or not
    if(ind==NULL)
    return e_failure;
    if(!strcmp(ind, ".txt"))
    {
        usleep(1000000);
        //printf("%s\n",encInfo->secret_fname);
        printf("yes,its txt file\n");
        encInfo->secret_fname=argv[3];
        strcpy(encInfo->extn_secret_file,".txt");
    }
    else if(!strcmp(ind, ".c"))
    {
        usleep(1000000);
        printf("yes,its c file\n");
        printf("%s\n",argv[3]);
        encInfo->secret_fname=argv[3];
        strcpy(encInfo->extn_secret_file,".c");
    }
    else if(!strcmp(ind, ".sh"))
    {
        usleep(1000000);
        printf("yes,its sh file\n");
        encInfo->secret_fname=argv[3];
        strcpy(encInfo->extn_secret_file,".sh");
    }
    else
    {
        return e_failure;
    }
    
    if (argv[4]!=NULL) //check the last arguments file is created or not
    {
      char*index1=strstr(argv[2],".");
        if(index1==NULL)
        return e_failure;
        if(strcmp(index,".bmp"))
        return e_failure;
        if(!strcmp(index1,".bmp"))
        {
            usleep(1000000);
            printf("yes,its bmp file\n");
            encInfo->stego_image_fname=argv[4];
        }
        else
        {
            return e_failure;
        }
    }
    else
    {
		FILE*ptr=fopen("Stego.bmp","w+");//defautly creating a file if file name is not passed
        usleep(1000000);
        printf("file will be created defaulty\n");
        encInfo->stego_image_fname="Stego.bmp";
	}
    return e_success;

}
Status do_encoding(EncodeInfo*encInfo)
{
	if(open_files(encInfo)==e_success)
    {
        usleep(1000000);
        printf("1.Open file is done\n");
        if(check_capacity(encInfo)==e_success)
        {
            usleep(1000000);
            printf("2.Check capacity function is done\n");
        }
        if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
        {
            usleep(1000000);
            printf("3.BMP header is Copied done\n");
            printf("offset: %ld\n",ftell(encInfo->fptr_stego_image));
            if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
            {
                usleep(1000000);
                printf("4.Magic string is encoded succesfully\n");
                printf("offset: %ld\n",ftell(encInfo->fptr_stego_image));
            }
            if(encode_secret_file_extn_size(encInfo->extn_secret_file,encInfo)==e_success)
            {
                usleep(1000000);
                printf("5.Secret file extension is added successfully\n");
                printf("offset: %ld\n",ftell(encInfo->fptr_stego_image));
            }
            if(encode_secret_file_extn(encInfo)==e_success)
            {
                usleep(1000000);
                printf("6.Secret file extension done\n");
                printf("offset: %ld\n",ftell(encInfo->fptr_stego_image));
            }
            if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
            {
                usleep(1000000);
                printf("7.Secret file size is  encoded successfully\n");
                printf("offset: %ld\n",ftell(encInfo->fptr_stego_image));
            }
            if(encode_secret_file_data(encInfo)==e_success)
            {
                usleep(1000000);
                printf("8.Secret file data are encoded successfuly\n");
                printf("offset: %ld\n",ftell(encInfo->fptr_stego_image));
            }
            if(copy_remaining_img_data(encInfo)==e_success)
            {
                usleep(1000000);
                printf("9.Remain data copied successfully\n");
                printf("offset: %ld\n",ftell(encInfo->fptr_stego_image));
            }
        }
        usleep(1000000);
    printf("<-----Encoding is successfully completed----->\n");
    }
    else
    {
    usleep(1000000);
    printf("<-----Encoding is not completed----->\n");
    }
}

