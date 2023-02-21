#include "acpi.h"
RSDPDescriptor* ACPI::rsd_ptr;
RSDT* ACPI::rsd_table;



void ACPI::initialize(){

    // Find the Root System Description Pointer
    ACPI::rsd_ptr = FindRSD();

    // is it valid
    int sum = 0;
    for (int i =0; i < 20 ; i++) {
        sum += ((char*)rsd_ptr)[i];
    }

    printf(" 0x%x sum\n", sum);
    return;

    //  Get the Root System Description Table
    RSDT* rootSystemDescriptionTable =  getRSDT((RSDPDescriptor *) rsd_ptr);

    auto tableHeader = &rootSystemDescriptionTable->h;

    // do checksum
    sum = 0;

    for(int i = 0; i < tableHeader->Length; i ++) {
        sum += ((char*) tableHeader)[i];
    }

    if( sum != 0)
        printf("Table invalid!");
}