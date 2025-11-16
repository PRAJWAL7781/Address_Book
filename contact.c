#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

int Name(char *name)
{
    for (int i = 0; name[i] != '\0'; i++)
    {
        
        if (((name[i] >= 'A' && name[i] <= 'Z') || (name[i] >= 'a' && name[i] <= 'z') || name[i] == ' ')==0)
        {
            return 0;
        }
    }
    return 1;
}

int Phone(char *phone)
{
    int len = strlen(phone);
    if (len != 10)
    {
        return 0;
    }

    for (int i = 0; i < len; i++)
    {
        if ((phone[i] >= '0' && phone[i] <= '9')==0)
            return 0;
    }
    
    return 1;
    
}

int Email(char *email)
{
    if (email[0] < 'a' || email[0] > 'z')
        return 0;

    char *a = strchr(email, '@');
    char *d = strstr(email, ".com");

    if (a && d && a < d && strcmp(d, ".com") == 0)
        return 1;

    return 0;
}


void swapcontacts(Contact *a,Contact *b)
{
    Contact temp=*a;
    *a=*b;
    *b=temp;
}

void listContacts(AddressBook *addressBook, int sortCriteria)
{
    if(addressBook->contactCount==0)
    {
        printf("no contact found\n");
        return;
    }
    // Sort contacts based on the chosen criteria
    for (int i = 0; i < addressBook->contactCount ; i++)
    {
        for (int j = i + 1; j < addressBook->contactCount; j++)
        {
            int compare = 0;

            if (sortCriteria == 1) // Name
                compare = strcmp(addressBook->contacts[i].name, addressBook->contacts[j].name);
            else if (sortCriteria == 2) // Phone
                compare = strcmp(addressBook->contacts[i].phone, addressBook->contacts[j].phone);
            else if (sortCriteria == 3) // Email
                compare = strcmp(addressBook->contacts[i].email, addressBook->contacts[j].email);

            if (compare > 0)
            {
                swapcontacts(&addressBook->contacts[i], &addressBook->contacts[j]);
            }
        }
    }
    //display
    for (int i = 0; i < addressBook->contactCount; i++)
    {
      
        printf("%3d) Name: %-20s | Phone: %-20s | Email: %-20s\n",
                i+1, addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}
int isDuplicate(AddressBook *addressBook, char *phone, char *email) {
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0 ||
            strcmp(addressBook->contacts[i].email, email) == 0) {
            return 1;
        }
    }
    return 0;
}

void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */
        int i = addressBook->contactCount;

    // Name
    do
    {
        printf("Enter name for contact: ");
        scanf(" %[^\n]", addressBook->contacts[i].name);
        if (Name(addressBook->contacts[i].name) == 0)
            printf("Invalid name. Please enter alphabets only.\n");
    } while (Name(addressBook->contacts[i].name) == 0);

    // Phone
    do {
    printf("Enter phone for contact: ");
    scanf(" %s", addressBook->contacts[i].phone);

    if (Phone(addressBook->contacts[i].phone) == 0) {
        printf("Invalid phone number. Must be 10 digits.\n");
    }
    else if (isDuplicate(addressBook, addressBook->contacts[i].phone, "")) {
        printf("Phone number already exists. Enter a different one.\n");
    }
    else {
        break;
    }
} while(1);



    // Email
    do {
        printf("Enter email for contact: ");
        scanf(" %s", addressBook->contacts[i].email);

        if (!Email(addressBook->contacts[i].email)) {
            printf("Invalid email. It must start with lowercase, contain '@', and end with '.com'\n");
        } else if (isDuplicate(addressBook, "", addressBook->contacts[i].email)) {
            printf("Email already exists. Enter a different one.\n");
        }else {
            break;
        }
    } while(1);

    // Increase count
    addressBook->contactCount++;
    printf("created successfully\n");
    }


int searchContact(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0)
    {
        printf("No contacts to search.\n");
        return -1;
    }

    char searchInput[50];
    printf("Enter name/phone/email to search: ");
    scanf(" %[^\n]", searchInput);

    int matches[50];
    int matchCount = 0;

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (strcasestr(addressBook->contacts[i].name, searchInput) ||
            strstr(addressBook->contacts[i].phone, searchInput) ||
            strcasestr(addressBook->contacts[i].email, searchInput))
        {
            matches[matchCount++] = i;
        }
    }

    // If no matches
    if (matchCount == 0)
    {
        printf("No contact found with '%s'.\n", searchInput);
        return -1;
    }

    // If one match
    if (matchCount == 1)
    {
        int index = matches[0];
        printf("\nContact Found:\n");
         printf("%3d) Name: %-20s | Phone: %-20s | Email: %-20s\n",
                index+1, addressBook->contacts[index].name,
                addressBook->contacts[index].phone,
                addressBook->contacts[index].email);
        return index;
    }

    // Multiple matches
    printf("\nMultiple contacts found:\n");
    int num=1;
    for (int i = 0; i < matchCount; i++)
    {
        int index = matches[i];
        printf("%3d) Name: %-20s | Phone: %-20s | Email: %-20s\n",
                num++, addressBook->contacts[index].name,
                addressBook->contacts[index].phone,
                addressBook->contacts[index].email);
    }

    // Ask user to newsearch by phone/email
    int newsearch;
    printf("\nEnter serial number to select a contact: ");
    scanf(" %d", &newsearch);
    num = 1;
    for (int i = 0; i < matchCount; i++)
    {
        int index = matches[i];
        if (num == newsearch)
        {
            printf("%3d) Name: %-20s | Phone: %-20s | Email: %-20s\n",
                num++, addressBook->contacts[index].name,
                addressBook->contacts[index].phone,
                addressBook->contacts[index].email);
            return index;
        }
        num++;
    }

    printf("No contact matched with the unique detail '%d'.\n", newsearch);
    return -1;
}

void editContact(AddressBook *addressBook)
{
    int index = searchContact(addressBook);

    if (index == -1) {
        printf("Contact not found.\n");
        return;
    }
    int choice;
    char new[100];
    do {
        printf("Edit the selected contact by\n ");
        printf("1. Name\n");
        printf("2. Phone\n");
        printf("3. Email\n");
        printf("4. Exit\n");
        printf("Enter the choice: ");
        scanf("%d",&choice);
        switch(choice)
        {
        case 1:
            do 
            {
                printf("Enter new name for contact: ");
                scanf(" %[^\n]", new);
                if (Name(new) == 0)
                    printf("Invalid name. Please enter alphabets only.\n");
            } while(Name(new) == 0);

            strcpy(addressBook->contacts[index].name, new);
            break;
        case 2:
	        do
            {
                printf("Enter new phone number for contact: ");
                scanf(" %s", new);
                if (Phone(new)==0) {
                printf("Invalid phone number. Must be 10 digits and not start with 0.\n");
                } else if (isDuplicate(addressBook, new, "")) {
                    printf("Phone number already exists. Enter a different one.\n");
                } else {
                    break;
                }
            } while (Phone(new) == 1);

            strcpy(addressBook->contacts[index].phone, new);
            break;
        case 3:
	        do
            {
                printf("Enter new email for contact: ");
                scanf(" %s", new);
                if (Email(new)==0) {
                printf("Invalid phone number. Must be 10 digits and not start with 0.\n");
                } else if (isDuplicate(addressBook, new, "")) {
                    printf("Phone number already exists. Enter a different one.\n");
                } else {
                    break;
                }
            } while (Email(new) == 1);

            strcpy(addressBook->contacts[index].email, new);
            break;
        case 4:
            break;
        default:
            printf("Invalid Entry");
        }
    }while (choice != 4);
}



void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
    int ret=searchContact(addressBook);
    int choice;
    printf("Are you sure want to delete the selected contact\n ");
    printf("1. Yes\n");
    printf("2. No\n");
    printf("Enter the choice: ");
    scanf("%d",&choice);
    switch(choice)
    {
        case 1:
            for(int i=ret;i<addressBook->contactCount;i++)
            {
                strcpy(addressBook->contacts[i].name,addressBook->contacts[i+1].name);
                strcpy(addressBook->contacts[i].phone,addressBook->contacts[i+1].phone);
                strcpy(addressBook->contacts[i].email,addressBook->contacts[i+1].email);
            }
            addressBook->contactCount--;
            printf("deleted successfully\n");
            break;
        case 2:
            break;
        default:
            printf("Invalid Entry");
    }
}