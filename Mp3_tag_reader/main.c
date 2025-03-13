#include<stdio.h>
#include"view.h"
#include<string.h>
#include"edit.h"
#include<stdlib.h>
int main(int argc,char*argv[])
{
    TagData data;
    editdata Data;
   if (argc < 2) 
    {
        display_help();
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0) 
    {
        display_help();
        return 0;
    } 
    else if (strcmp(argv[1], "-v") == 0 && argc == 3) 
    {
        if(view_tags(argv,&data)!=success) // calling the function view tags
        {
            printf("<<<---------FAILED TO VIEW TAGS---------->>>\n");
        }
        else
        {
        printf("<<<---------DETAILS DISPLAYED SUCCESSFULLY--------->>>\n");
        return failure;
        }
    } 
    else if (strcmp(argv[1], "-e") == 0 && argc == 5) 
    {
        char *tag = argv[2];
        char *filename = argv[3];
        if( strstr(filename , ".mp3"))// we have to check whether file extension .mp3 or not
        {
            printf("Pass valid arguments\n");
            display_help();
            exit(0);
        }
        char *value = argv[4];
        if( strstr(value , ".mp3"))
        {
            if (edit_tag(filename, tag, value,&Data) != 1)  // edtt function calling
            {
                printf("\n<<<-------FAILED TO EDIT TAG------->>>\n");
            }
            else 
            {
                printf("\n<<<-------TAG EDITED SUCCESSFULLY------->>>\n");
            }
        }
        else
        {
            printf("Pass valid arguments\n");
            display_help();
            exit(0);
        }
    } 
    else 
    {
        display_help();
        return 1;
    }

    return 0;
}
void display_help() 
{
    printf("\nUsage: mp3tagreader [options] filename\n");
    printf("Options:\n");
    printf("  -h                  Display help\n");
    printf("  -v                  View tags\n");
    printf("  -e         <value>  Edit tags\n");
    printf("  -a --> Artist   -A --> Alblum\n");
    printf("  -y --> Year     -m --> Content\n");
    printf("  -t --> Title    -c --> Comments\n");
}
