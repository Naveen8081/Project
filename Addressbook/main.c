/*
Name:P.Naveen
Date:01-12-2024
Description:Storing contacts details of different person
*/
#include <stdio.h>
#include "contact.h"
#include<stdio_ext.h>
int main() 
{
    int choice;
    AddressBook s;
    initialize(&s); // Initialize the address book

    do 
    {
        printf("\nAddress Book Menu:\n");
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        __fpurge(stdin);
        switch (choice) {
            case 1:
                createContact(&s);
                break;
            case 2:
                searchContact(&s);
                break;
            case 3:
                editContact(&s);
                break;
            case 4:
                deleteContact(&s);
                break;
            case 5:
                listContacts(&s);
                break;
            case 6:
                printf("Saving and Exiting...\n");
                saveContactsToFile(&s);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
    
       return 0;
}
