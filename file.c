#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fp = fopen("contact.txt", "w");
    if (fp == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < addressBook->contactCount; i++) {
        fprintf(fp, "%s,%s,%s\n", 
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fp);
    printf("Contacts saved successfully.\n");
}

void loadContactsFromFile(AddressBook *addressBook) {
    FILE *fp = fopen("contact.txt", "r");
    if (fp == NULL) {
        printf("No saved contacts found.\n");
        return;
    }

    addressBook->contactCount = 0;

    while (fscanf(fp, " %[^,], %[^,], %[^\n]\n", 
                  addressBook->contacts[addressBook->contactCount].name,
                  addressBook->contacts[addressBook->contactCount].phone,
                  addressBook->contacts[addressBook->contactCount].email) == 3) 
    {
        addressBook->contactCount++;

        if (addressBook->contactCount >= MAX_CONTACTS)
            break;
    }

    fclose(fp);
    printf("Contacts loaded successfully.\n");
}