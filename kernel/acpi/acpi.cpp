#include "acpi.h"
#include "../../CoreLib/Memory.h"
#include "../memory/VirtualMemoryManager.h"

RSDPDescriptor* ACPI::rsd_ptr;
RSCPDescriptor20* ACPI::rsd2_ptr;
RSDT* ACPI::rsd_table;

const int KERNEL_OFFSET = 0xC0000000;

void ACPI::initialize(){
    // Find the Root System Description Pointer
    ACPI::rsd_ptr = FindRSD();
    printf("RSD address: 0x%x\n", ACPI::rsd_ptr);
    printRSD(rsd_ptr);


    if( rsd_ptr->Revision == 0 ){
        // Using version 1.0 of the ACPI specification
        int sum = rsd_ptr->Checksum;
        for (int i =0; i < sizeof(RSDPDescriptor) ; i++) {
            sum += ((char*)rsd_ptr)[i];
        }

        printf(" 0x%x sum\n", sum);
        if(sum & 0xfff0)
            printf("valid rsd!\n");
        else
            printf("invalid rsd\n");

        printf("rsdp: 0x%x\n", rsd_ptr);

        printf("0x%x address\n", (rsd_ptr->RsdtAddress));
        Immediate_Map(rsd_ptr->RsdtAddress + KERNEL_OFFSET, rsd_ptr->RsdtAddress);

        RSDT* rootSystemDescriptionTable = (RSDT*)(rsd_ptr->RsdtAddress + KERNEL_OFFSET);
        //printf("0x%x Root System Descriptor address\n", rootSystemDescriptionTable);
        // checksum it, but we'll ignore it for now
        printf("signature ");
        for (int i = 0; i < 4; i++) {
            kterm_put( rootSystemDescriptionTable->h.Signature[i]);
        }
        kterm_put('\n');


        int entries = (rootSystemDescriptionTable->h.Length - sizeof (rootSystemDescriptionTable->h)) /4;
        printf("%d num entries\n", entries);
        for( int i = 0; i < entries; i++){
            ACPISDTHeader* h = (ACPISDTHeader*) rootSystemDescriptionTable->PointerToSDT + i ;
            if(strncmp(h->Signature, "FACP", 4)){
                printf("Found FACP Entry!\n");
            }
        }


    } else{
        // parse it as of version2.0
        printf("rsd2_ptr\n");
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