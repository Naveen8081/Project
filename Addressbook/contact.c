#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"
#include<stdio_ext.h>
void listContacts(AddressBook *s) 
{
    {
     if(s->contactCount>0)//contactcount is zero then no contact in database
    {
        for(int i=0;i<s->contactCount;i++)
        {
            printf("Name:%s\n",s->contacts[i].name);
            printf("Mobile_no:%s\n",s->contacts[i].phone);
            printf("Email_id:%s\n",s->contacts[i].email);
        }
    }
    else
    {
        printf("\t\tThere is no contacts in database\n");
    }
}
    
}

    void initialize(AddressBook *s) {
        s->contactCount = 0;
    
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(s);//load to the file
}

void createContact(AddressBook *s)
{
    char str1[100];
    char str2[100];
    char str3[100];
    
     name:
            printf("Enter your name: ");
            scanf("%[^\n]", s->contacts[s->contactCount].name);
            getchar();
            strcpy(str1, s->contacts[s->contactCount].name);
            if (!my_name(str1)) //if name containg digits or something 
            {
                printf("Enter a valid name!\n");
                goto name;
            }
            else
            {
                printf("Name: %s\n",str1);
            }
    number:
            printf("Enter your mobile number: ");
            scanf("%s",s->contacts[s->contactCount].phone);
            //printf("%s",s->contacts[s->contactCount].phone);
            strcpy(str2,s->contacts[s->contactCount].phone);
            int len=strlen(str2);
             int res1 = my_number(str2,len,s);//it is validiating email id
            if (res1 == -1) 
            { 
                goto number;
            } 
            else if (res1 == 0) 
            { 
                printf("Enter a valid mobile_no!\n");
                goto number;
            } 
            else 
            {
                printf("number: %s\n", str2); 
                
            }
    email:
            printf("Enter your email-id: ");
            scanf("%s", s->contacts[s->contactCount].email);
            strcpy(str3, s->contacts[s->contactCount].email);
            int res = my_email(str3, s);//it is validiating email id
            if (res == -1) 
            { 
                goto email;
            } 
            else if (res == 0) 
            { 
                printf("Enter a valid email!\n");
                goto email;
            } 
            else 
            {
                printf("email: %s\n", str3); 
                
            }

                (s->contactCount)++;
                //printf("%d",s->contactCount);

}
int my_name(char str1[]) 
{

    for (int i = 0; str1[i]; i++) 
    {
        if(!(isalpha(str1[i])))//isalpha function returns alaphabetic found means it returns 1 or else return 0
        {
            return 0; // Invalid character found
        }
    }
    return 1; // All characters are found in alphabetic characters
}

int my_number(char str2[],int len,AddressBook*s)
{
    
    if (len != 10) //mobile number must be 10 digits only
    {
        return 0;
    }
    for (int i = 0; str2[i]; i++) 
    {
        if (!(str2[i] >= '0' && str2[i] <= '9')) //it is containing only digits
        {
            return 0;   
        }
    }
    for (int i = 0; i < s->contactCount; i++) 
    {
        if (strcmp(str2, s->contacts[i].phone) == 0) //it is mobile is unique or not
        {
            printf("This Mobile_no is already exist!!\n");
            return -1;
        }
    }
    return 1; 
}
int my_email(char str3[], AddressBook *s) {
    char *ind;

    // Check if the email already exists
    for (int j = 0; j < s->contactCount; j++) 
    {
        if (strcmp(str3, s->contacts[j].email) == 0) 
        {
            printf("This email already exists!\n");
            return -1; //  return for unique
        }
    }
    for (int i = 0; str3[i]; i++) 
    {
        if (str3[i] == '@') 
        {
            if ((str3[i - 1] >= '0' && str3[i - 1] <= '9') || isalpha(str3[i - 1])) //it is checking @ previous index is digit or alphabetic present or not
            {
                ind = strstr(str3, ".com");//strstr return some address so collect in pointer variable
                if (ind && isalpha(*(ind - 1))) 
                {
                    return 1; // Valid email
                }
            }
        }
    }
    return 0; // Invalid email 
}
void searchContact(AddressBook *s) 
{
    
    int choice;
    printf("\t\tSEARCHING CONTACT\n");
    printf("1.Search by name.\n");
    printf("2.Search by Mobile Number.\n");
    printf("3.Search by Email_id\n");
    scanf("%d",&choice);
    __fpurge(stdin);
    switch(choice)
    {
        case 1:
           search_name(s);
           break;
        case 2:
            search_mobile(s);
            break;
        case 3:
            search_email(s);
            break;

    } 

}
int search_name(AddressBook *s) 
{
    char name[100];
    printf("Enter the name: ");
    scanf("%s", name);
    int count = 0;
    int index;
    for (int i = 0; i < s->contactCount; i++) // Search for contacts matching the name
    {
        if (strcmp(name, s->contacts[i].name) == 0)
        {
            printf("Name: %s, Mobile: %s, Email: %s\n",s->contacts[i].name, s->contacts[i].phone, s->contacts[i].email);
            count++;
            index=i;
        }
    }
    if (count == 0) 
    {
        printf("No contacts found with the name '%s'.\n", name);
        return 1; // No matches found
    }
    // if (count == 1) 
    // {
    //     printf("\nContact Found:\n");
    //     printf("Name : %s\n", s->contacts[index].name);
    //     printf("Mobile No : %s\n", s->contacts[index].phone);
    //     printf("Email ID : %s\n", s->contacts[index].email);
    //     return index; // Return the index of the single match
    // }
    printf("\nMultiple contacts found. Please refine your search:\n");
    printf("1. Search by mobile number\n");
    printf("2. Search by email ID\n");
    printf("Enter your choice: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 1) 
    {
        char mobile[15];
        printf("Enter the mobile number: ");
        scanf("%s", mobile);
        for (int i = 0; i < s->contactCount; i++) 
        {
            if (strcmp(name, s->contacts[i].name) == 0 &&
                strcmp(mobile, s->contacts[i].phone) == 0) 
                {
                printf("\nContact Found:\n");
                printf("Name : %s\n", s->contacts[i].name);
                printf("Mobile No : %s\n", s->contacts[i].phone);
                printf("Email ID : %s\n", s->contacts[i].email);
                return i; // Return the index of the matching contact
                }
        }
        printf("No contact found with the mobile number '%s'.\n", mobile);
    } 
    else if (choice == 2) 
    {
        char email[100];
        printf("Enter the email ID: ");
        scanf("%s", email);

        for (int i = 0; i < s->contactCount; i++) 
        {
            if (strcmp(name, s->contacts[i].name) == 0 &&
                strcmp(email, s->contacts[i].email) == 0) 
                {
                printf("\nContact Found:\n");
                printf("Name : %s\n", s->contacts[i].name);
                printf("Mobile No : %s\n", s->contacts[i].phone);
                printf("Email ID : %s\n", s->contacts[i].email);
                return i; // Return the index of the matching contact
            }
        }
        printf("No contact found with the email ID '%s'.\n", email);
    } 
    else 
    {
        printf("Invalid choice.\n");
    }

    return -1; // No matches 
}

void search_mobile(AddressBook *s)
{
   int flag=1; 
            char str[100];
            printf("Enter the Mobile_NO: ");
            scanf("%s",str);
            for(int i=0;i<s->contactCount;i++)
            {
                if(!(strcmp(str,s->contacts[i].phone)))//comparing present mobile no and file phone no
                {
                    flag=0;
                    printf("Name:%s\n",s->contacts[i].name);
                    printf("Mobile_no:%s\n",s->contacts[i].phone);
                    printf("Email_id:%s\n",s->contacts[i].email);
                    return;
                }
            }
                if(flag)
                {
                    printf("No contact found with the Mobile_no '%s'.\n", str);
                        
                }
                    
}
void search_email(AddressBook *s)
{
    int flag=1;

            char str[100];
            printf("Enter the Email_id: ");
            scanf("%s",str);
                for(int i=0;i<s->contactCount;i++)//s->contact is actually size
                {
                    if(!(strcmp(str,s->contacts[i].email)))
                    {
                        flag=0;
                        printf("Name:%s\n",s->contacts[i].name);
                        printf("Mobile_no:%s\n",s->contacts[i].phone);
                        printf("Email_id:%s\n",s->contacts[i].email);
                        return;
                    }

                }
                if(flag)
                {
                    printf("No contact found with the Email_id '%s'.\n", str);
                    
                }
                    
}    


void editContact(AddressBook*s)
{
    int  choice;
    printf("\t\t\tEDIT CONTACT\n");
    printf("1.Edit Name\n");
    printf("2.Edit Mobile_no\n");
    printf("3.Edit Email_id\n");
    scanf("%d",&choice);
     __fpurge(stdin);
    switch(choice)
    {
        case 1:
           edit_name(s);
           break;
        case 2:
            edit_mobile(s);
            break;
        case 3:
            edit_email(s);
            break;

    } 
}
void edit_name(AddressBook *s) 
{
    int index = search_name(s); // Use search_name to find the contact
    if (index == 1) 
    {
        printf("No matching contact.\n");
        return;
    }
    char str[100];
    while (1) 
    {
        printf("Enter the updated name: ");
        scanf("%s", str);

        if (!my_name(str)) 
        { 
            printf("Invalid name. Please enter a valid name.\n");
        } else 
        {
            strcpy(s->contacts[index].name, str);
            printf("Name updated successfully to '%s'.\n", s->contacts[index].name);
            break;
        }
    }
}


void edit_mobile(AddressBook *s)
{
    int flag=0;
            char str[100];
            printf("Enter the Mobie_no: ");
            scanf("%s",str);
            char str1[100];
            for(int i=0;i<s->contactCount;i++)
                {
                    if(!(strcmp(str,s->contacts[i].phone)))
                        {
                            flag=1;
                            do{
                            printf("Enter the update mobile number: ");
                            scanf("%s",str1);
                            int len=strlen(str1);
                            if(!my_number(str1,len,s))//it is validating present given mobile no 
                            {
                                printf("Enter a valid Mobile_no!\n");                                           
                            }  
                            else
                            {      
                            strcpy(s->contacts[i].phone,str1);
                            printf("Mobile_no:%s\n",s->contacts[i].phone);
                            printf("Updated successfully\n");
                            break;
                            }
                            }while(1);
                            return;
                        }
                }
                 if(!flag)
                {
                    printf("No contact found with the Mobile_no\n");
                    return;
                }
             
}
void edit_email(AddressBook *s)
{
    int flag=0;
            char str[100];
            printf("Enter the Email_id: ");
            scanf("%s",str);
            char str1[100];
            for(int i=0;i<s->contactCount;i++)
            {
                if(!(strcmp(str,s->contacts[i].email)))
                {
                    flag=1;
                    do{
                    printf("Enter your update email_id: ");
                    scanf("%s",str1);
                    if(!my_email(str1,s)) //it is validating present given mail id or not
                    {
                        printf("Enter a valid Email_id!\n");
                    }
                    else{
                    strcpy(s->contacts[i].email,str1);
                    printf("Email_id:%s\n",s->contacts[i].email);
                    printf("Updated successfully\n");
                    break;
                    }
                    }while(1);
                    return;
                }
            }
                    if(!flag)
                    {
                        printf("No contact found with the Email_id '%s'.\n", str);
                        return;
                    }
            
}

void deleteContact(AddressBook *s) 
{
    int index = search_name(s); // Use the function to find the contact index
    if (index == 1) 
    {
        //printf("No matching contact found..!\n");
        return;
    }
    for (int i = index; i < s->contactCount ; i++) 
    {
        strcpy(s->contacts[i].name, s->contacts[i + 1].name);
        strcpy(s->contacts[i].email, s->contacts[i + 1].email);
        strcpy(s->contacts[i].phone, s->contacts[i + 1].phone);
    }

    // Decrease the contact count
    s->contactCount--;
    printf("Contact deleted successfully.\n");
}



   
