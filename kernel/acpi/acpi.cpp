#include "acpi.h"
RSDPDescriptor* ACPI::rsd_ptr;
RSCPDescriptor20* ACPI::rsd2_ptr;
RSDT* ACPI::rsd_table;



void ACPI::initialize(){

    // Find the Root System Description Pointer
    ACPI::rsd_ptr = FindRSD();
    printf("RSD address: 0x%x\n", ACPI::rsd_ptr);
    //printRSD(rsd_ptr);


    if( rsd_ptr->Revision == 0 ){
        // Using veriosn 1.0 of the ACPI specifiction
        int sum = rsd_ptr->Checksum;
        for (int i =0; i < sizeof(RSDPDescriptor) ; i++) {
            sum += ((char*)rsd_ptr)[i];
        }

        printf(" 0x%x sum\n", sum);
        if(sum & 0xfff0)
            printf("valid rsd!\n");
        else
            printf("invalid rsd\n");


        //  Get the Root System Description Table NOTE: might need memory mapping
        //RSDT* rootSystemDescriptionTable = (RSDT*) (rsd_ptr->RsdtAddress + 0xC0000000);
    } else{
        // parse it as of version2.0
        ACPI::rsd2_ptr = (RSCPDescriptor20*)rsd_ptr;
    }



/*
    auto tableHeader = &rootSystemDescriptionTable->h;

    // do checksum
    sum = 0;

    for(int i = 0; i < tableHeader->Length; i ++) {
        sum += ((char*) tableHeader)[i];
    }

    if( sum != 0)
        printf("Table invalid!");*/
}