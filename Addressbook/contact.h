#ifndef CONTACT_H
#define CONTACT_H

    #define MAX_CONTACTS 100

    typedef struct {
        char name[50];
        char phone[20];
        char email[50];
    } Contact;//it is datatype

    typedef struct {
        Contact contacts[MAX_CONTACTS];
        int contactCount;
    } AddressBook;//it is datatype

void createContact(AddressBook*);
int my_name(char[]);
int my_number(char[],int,AddressBook*);
int my_email(char[],AddressBook*);
int search_name(AddressBook*);
void search_mobile(AddressBook*);
void search_email(AddressBook*);
void searchContact(AddressBook*);
void edit_name(AddressBook*);
void edit_email(AddressBook*);
void edit_mobile(AddressBook*);
void editContact(AddressBook*);
void deleteContact(AddressBook*);
void listContacts(AddressBook*);
void initialize(AddressBook*);
void saveContactsToFile(AddressBook*);

#endif
