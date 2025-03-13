#include<stdio.h>
#include"view.h"
#include <string.h>
#include <stdlib.h>
Status view_tags(char*argv[],TagData*data)
{
    printf("<<<-----------SELECTED VIEW DETAILS------------->>>\n");
    if(read_validate(argv,data))
    {
        //printf("Read and validate is done\n");
    }
    if(check_ID3_present(data))
    {
        //printf("Check Tags is completed\n");
        if(check_TIT2_tag(data))
        //printf("Checked TIT2 tag is done\n");
        if(check_TPE1_tag(data))
        //printf("checked TPE1 tag is done\n");
        if(check_TALB_tag(data));
        //printf("TALB tag checked done\n");
        if(check_TYER_tag(data))
        //printf("TYER tag is checked done\n");
        if(check_TCON_tag(data))
        //printf("TCON is done\n");
        if(check_COMM_tag(data))
        //printf("COMM tag is checked done\n");
        return success;
    }
    return failure;
}
int read_validate(char*argv[],TagData*data)
{
    char*index=strstr(argv[2],".");
    if(index==NULL)
    {
        printf("File should be presented with dot\n");
        exit(0);
    }
   
    if(!strcmp(index,".mp3")) //  we have to check whether file extension .mp3 or not
    {
        data->fptr_image=fopen(argv[2],"r"); 
    }
    if(data->fptr_image==NULL)
    {
        printf("ERROR:Unable open the file \n");
        exit(0);
    }
    else
    //printf("File opened successfully\n");
    return 1;
}
int check_ID3_present(TagData*data)
{
    char buffer[3];
    fread(buffer,sizeof(char),3,data->fptr_image);
    buffer[3]='\0';
    // printf("TAG: %s\n",buffer);
    // printf("TAG: %02x %02x %02x\n", buffer[0], buffer[1], buffer[2]);
    // int pos=ftell(data->fptr_image);
    //printf("OFFSET: %d\n",pos);
    data->version=buffer;//store the version of song 
    if(!strcmp(buffer,"ID3"))
    {
        printf("\n----------------------------------------------------\n");
        printf("\tMP3 TAG READER AND EDITOR FOR ID3V2\n");
        printf("----------------------------------------------------\n");
        //printf("Yes,it ID3 tag\n");
        fseek(data->fptr_image,7,SEEK_CUR);// 7 bytes of header
    }
    else
    {
        printf("No,it is an ID3 tag\n");
        return 0;
    }
    return 1;
}
int check_TIT2_tag(TagData*data)
{
    char buffer1[4];
    fread(buffer1,sizeof(char),4,data->fptr_image);
    //printf("TAG: %02x %02x %02x %02x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    buffer1[4]='\0';
    //printf("TAG: %s\n",buffer1);
    // int pos=ftell(data->fptr_image);
    // printf("OFFSET: %d\n",pos);
    if(!strcmp(buffer1,"TIT2"))
    {
        //printf("Yes it is TIT2 version\n");
        char size[4];
        fread(size,sizeof(char),4,data->fptr_image);
        int TIT2_size=convert_big_little_endian(4,size);// stored in a big endian we have to change into little endian
        TIT2_size--;
        //printf("%d\n",TIT2_size);
        fseek(data->fptr_image,3,SEEK_CUR); // 3 bytes of flag
        char buffer[TIT2_size+1];
        fread(buffer,sizeof(char),TIT2_size,data->fptr_image);
        buffer[TIT2_size]='\0';
        data->title=buffer; // update data title with buffer
        printf("TITLE\t\t: \t%s\n",data->title);
        return 1;
    }
    else
    {
        printf("No it is TIT2 version\n");
        return 0;
    }
}

int convert_big_little_endian(int size,char*buffer)
{
    char temp;
    char *ptr=buffer;
    char*end=ptr+size-1;
    while(ptr<end)
    {
        temp=*ptr;
        *ptr=*end;
        *end=temp;
        ptr++,end--;
    }
    return buffer[0];
}
int check_TPE1_tag(TagData*data)
{
    char buffer[4];
    fread(buffer,sizeof(char),4,data->fptr_image);
    //printf("TAG: %02x %02x %02x %02x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    buffer[4]='\0';
    if(!strcmp(buffer,"TPE1"))
    {
        //printf("Yes it is TPE1 Tag\n");
        char size[4];
        fread(size,sizeof(char),4,data->fptr_image);
        int TPE1_size=convert_big_little_endian(4,size);
        TPE1_size--;
        //printf("%d\n",TPE1_size);
        fseek(data->fptr_image,3,SEEK_CUR);
        char buffer[TPE1_size+1];
        fread(buffer,sizeof(char),TPE1_size,data->fptr_image);
        buffer[TPE1_size]='\0';
        data->artist=buffer;// update data title with buffer
        printf("ARTIST\t\t: \t%s\n",data->artist);
        return 1;
    }
    else
    {
        printf("No it is TPE1 Tag\n");
        return 0;
    }
}
int check_TALB_tag(TagData*data)
{
    char buffer[4];
    fread(buffer,sizeof(char),4,data->fptr_image);
    //printf("TAG: %02x %02x %02x %02x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    buffer[4]='\0';
    //printf("TAG: %s\n",buffer);
    //int pos=ftell(fptr_image);
    //printf("OFFSET: %d\n",pos);
    if(!strcmp(buffer,"TALB"))
    {
        //printf("Yes TALB tag is found\n");
        char size[4];
        fread(size,sizeof(char),4,data->fptr_image);
        int TALB_size=convert_big_little_endian(4,size);
        TALB_size--;
        //printf("%d\n",TPE1_size);
        fseek(data->fptr_image,3,SEEK_CUR);
        char buffer[TALB_size+1];
        fread(buffer,sizeof(char),TALB_size,data->fptr_image);
        buffer[TALB_size]='\0';
        data->album=buffer;// update data album with buffer
        printf("ALBUM\t\t: \t%s\n",data->album);
        return 1;
    }
    else
    {
        printf("No TALB tag not found\n");
        return 0;
    }   
}
int check_TYER_tag(TagData*data)
{
    char buffer[4];
    fread(buffer,sizeof(char),4,data->fptr_image);
    //printf("TAG: %02x %02x %02x %02x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    buffer[4]='\0';
    // printf("TAG: %s\n",buffer);
    // int pos=ftell(fptr_image);
    // printf("OFFSET: %d\n",pos);
    if(!strcmp(buffer,"TYER"))
    {
        //printf("Yes TYER tag is found\n");
        char size[4];
        fread(size,sizeof(char),4,data->fptr_image);
        int TYER_size=convert_big_little_endian(4,size);
        TYER_size--;
        //printf("%d\n",TPE1_size);
        fseek(data->fptr_image,3,SEEK_CUR);
        char buffer[TYER_size+1];
        fread(buffer,sizeof(char),TYER_size,data->fptr_image);
        buffer[TYER_size]='\0';
        data->year=buffer;// update year title with buffer
        printf("YEAR\t\t: \t%s\n",data->year);
        return 1;
    }
    else
    {
        printf("No TYER tag not found\n");
        return 0;
    }   
}
int check_TCON_tag(TagData*data)
{
    char buffer[4];
    fread(buffer,sizeof(char),4,data->fptr_image);
    //printf("TAG: %02x %02x %02x %02x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    buffer[4]='\0';
    // printf("TAG: %s\n",buffer);
    // int pos=ftell(fptr_image);
    // printf("OFFSET: %d\n",pos);
    if(!strcmp(buffer,"TCON"))
    {
       // printf("Yes TCON tag is found\n");
        char size[4];
        fread(size,sizeof(char),4,data->fptr_image);
        int TCON_size=convert_big_little_endian(4,size);
        TCON_size--;
        //printf("%d\n",TPE1_size);
        fseek(data->fptr_image,3,SEEK_CUR);
        char buffer[TCON_size+1];
        fread(buffer,sizeof(char),TCON_size,data->fptr_image);
        buffer[TCON_size]='\0';
        data->content=buffer;// update data content with buffer
        printf("CONTENT\t\t: \t%s\n",data->content);
        return 1;
    }
    else
    {
        printf("No TCON tag not found\n");
        return 0;
    }   
}
int check_COMM_tag(TagData*data)
{
    char buffer[4];
    fread(buffer,sizeof(char),4,data->fptr_image);
    //printf("TAG: %02x %02x %02x %02x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    buffer[4]='\0';
    // printf("TAG: %s\n",buffer);
    // int pos=ftell(fptr_image);
    // printf("OFFSET: %d\n",pos);
    if(!strcmp(buffer,"COMM"))
    {
        //printf("Yes COMM tag is found\n");
        char size[4];
        fread(size,sizeof(char),4,data->fptr_image);
        int COMM_size=convert_big_little_endian(4,size);
        COMM_size--;
        //printf("%d\n",COMM_size);
        fseek(data->fptr_image,3,SEEK_CUR);
        char buffer[COMM_size+1];
        fread(buffer,sizeof(char),COMM_size,data->fptr_image);
        buffer[COMM_size]='\0';
        data->comment=buffer;// update data comment with buffer
        //buffer[3]='!';
        printf("COMMENT\t\t: \t%s\n",data->comment);
        printf("----------------------------------------------------\n");
        return 1;
    }
    else
    {
        printf("No COMM tag not found\n");
        return 0;
    }   
}
