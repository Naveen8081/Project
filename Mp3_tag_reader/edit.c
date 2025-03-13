#include <stdio.h>
#include <string.h>
#include "edit.h"
#include "view.h"
#include <stdlib.h>
int edit_tag(const char *filename, const char *tag, const char *value, editdata *Data)
{
    printf("\n<<<---------SELECTED EDIT OPTION--------->>>\n");
    char *index = strstr(tag, "-");
    if (index == NULL)
    {
        printf("\n<<<------ENTER THE VALID TAG------>>>\n");
        return 0;
    }
    Data->wptr = fopen("Temp.mp3", "w+");
    Data->fptr = fopen(value, "r");
    if (Data->fptr == NULL || Data->wptr == NULL)
    {
        printf("\n<<<-----ERROR:UNABLE TO OPEN FILE------>>>\n");
        exit(0);
    }
    char buffer[10];
    fread(buffer, sizeof(char), 10, Data->fptr);
    fwrite(buffer, sizeof(char), 10, Data->wptr);
    // printf("%ld\n",ftell(Data->wptr));
    char tag1[5] = {0};
    int flag = 0;
    // we have to check which tag need to edit
    if (strcmp(index, "-t") == 0)
    {
        printf("\n<<<---------SELECTED TITLE OPTION--------->>>\n");
        strncpy(tag1, "TIT2", 4);
        printf("\nTITLE : %s\n",filename);
        flag = 1;
    }
    else if (strcmp(index, "-a") == 0)
    {
        printf("\n<<<---------SELECTED ARTIST OPTION--------->>>\n");
        strncpy(tag1, "TPE1", 4);
        printf("\nARTIST : %s\n",filename);
        flag = 1;
    }
    else if (strcmp(index, "-A") == 0)
    {
        printf("\n<<<---------SELECTED ALBUM OPTION--------->>>\n");
        strncpy(tag1, "TALB", 4);
        printf("\nALBUM : %s\n",filename);
        flag = 1;
    }
    else if (strcmp(index, "-y") == 0)
    {
        printf("\n<<<---------SELECTED YRAR OPTION--------->>>\n");
        strncpy(tag1, "TYER",4);
        printf("\nYEAR : %s\n",filename);
        flag = 1;
    }
    else if (strcmp(index, "-m") == 0)
    {
        printf("\n<<<---------SELECTED CONTENT OPTION--------->>>\n");
        strncpy(tag1, "TCON",4);
        printf("\nCONTENTS : %s\n",filename);
        flag = 1;
    }
    else if (strcmp(index, "-c") == 0)
    {
        printf("\n<<<---------SELECTED COMMENTS OPTION--------->>>\n");
        strncpy(tag1, "COMM",4);
        printf("\nCOMMENTS : %s\n",filename);
        flag = 1;
    }
    if (!flag)
    {
        display_help();
        return 0;
    }
    for (int i = 0; i < 6; i++)
    {
        char buffer[5] = {0}; 
        fread(buffer, 4, 1, Data->fptr);
        buffer[4] = '\0'; // read the 4 byte
        //printf("Buffer: %s, Tag1: %s\n", buffer, tag1);
        if (!strncmp(buffer, tag1, 4))
        {
            fwrite(buffer,4,1,Data->wptr); // if matched with tag
            //printf("Match found for Tag1: %s\n", tag1);
            char buffer1[4];
            fread(buffer1, sizeof(char), 4, Data->fptr); // read next 4 byte
            int old_size = big_to_little_endian(4, buffer1);
            //printf("Old size: %d\n", old_size);
            int mod_size = strlen(filename); // we have to find length of update name 
            int modify_size = little_to_big_endian(mod_size + 1); // that updated need to store big endian format
            //printf("NEW SIZE: %X\n", modify_size);
            fwrite(&modify_size, sizeof(char), 4, Data->wptr);
            //printf("SIZE OFFSET:%ld\n", ftell(Data->wptr));

            // writing  the filename
            char buffer2[3];
            fread(buffer2, sizeof(char), 3, Data->fptr);
            fwrite(buffer2, sizeof(char), 3, Data->wptr); // 3 bytes of flag
            //printf("OFFSET: %ld\n", ftell(Data->wptr));
            fwrite(filename, sizeof(char), mod_size, Data->wptr);  // we have to write updated data
            fseek(Data->fptr, old_size - 1, SEEK_CUR); // move until old size
            // printf("File pointer after skipping offset: %ld\n", ftell(Data->wptr));
        }
        else
        {
            //printf("No match for Tag1: %s\n", tag1);
            fwrite(buffer, 4, 1, Data->wptr); // if it is not matching
            char size_buffer[4];
            fread(size_buffer, sizeof(char), 4, Data->fptr); 
            fwrite(size_buffer, sizeof(char), 4, Data->wptr);
            char flag[3];
            fread(flag, sizeof(char), 3, Data->fptr); // 3 bytes of flag
            fwrite(flag, sizeof(char), 3, Data->wptr);
            int modify_size = (big_to_little_endian(4, size_buffer));
            char tag[modify_size+1];
            fread(tag, modify_size-1, 1, Data->fptr);
            fwrite(tag, modify_size-1, 1, Data->wptr);
            //printf("SIZE OFFSET:%ld\n", ftell(Data->wptr));
        }
    }
    // copy remaining bytes
    char array[500];
    while(fread(array,sizeof(char),sizeof(array),Data->fptr)>0)
    {
        fwrite(array,sizeof(char),sizeof(array),Data->wptr);
    }
    remove(value);
    rename("Temp.mp3",value);
    return 1;
}

int big_to_little_endian(int size, char *buffer)
{
    char temp;
    char *ptr = buffer;
    char *end = ptr + size - 1;
    while (ptr < end)
    {
        temp = *ptr;
        *ptr = *end;
        *end = temp;
        ptr++, end--;
    }
    return buffer[0];
}
union bigendian
{
    char big_end[4];
    int lil_end;
};
int little_to_big_endian(int size)
{
    union bigendian a;
    char *temp = (char *)&size;
    for (int i = 3; i >= 0; i--)
    {
        a.big_end[3 - i] = *(temp + i);
    }
    return a.lil_end;
}
