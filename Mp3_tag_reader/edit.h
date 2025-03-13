#include<stdio.h>

typedef struct
{
    FILE*wptr;
    FILE*fptr;

}editdata;


int edit_tag(const char *filename, const char *tag, const char *value,editdata*Data);

/*To convert little endian to big endian*/
int big_to_little_endian(int size,char*array);

/*To convert big to little endian*/
int little_to_big_endian(int size);
