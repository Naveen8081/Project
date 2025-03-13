#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *s) 
{
    FILE*fptr=fopen("contact.csv","w");//open the file with read and write
    fprintf(fptr,"#%d",s->contactCount);
    for(int i=0;i<s->contactCount;i++)
    {
        fprintf(fptr,"\n%s,%s,%s",s->contacts[i].name,s->contacts[i].email,s->contacts[i].phone);//print the contact in file
    }
    fclose(fptr);
}

void loadContactsFromFile(AddressBook *s) 
{
    FILE*fptr=fopen("contact.csv","r");
    if(fptr==NULL)
    {
        printf("FILE IS NOT CREATED\n");
        return;
    }
    fscanf(fptr,"#%d",&s->contactCount);
    for(int i=0;i<s->contactCount;i++)
    {
        fscanf(fptr,"\n%[^,],%[^,],%[^\n]",s->contacts[i].name,s->contacts[i].email,s->contacts[i].phone);//it is reading contacts from file
    }
    fclose(fptr);//closing file
}
