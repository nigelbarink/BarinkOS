#include "rsdp.h"

void printRSD(RSDPTR* rsd){
    printf("Signature: ");
    for(int i = 0; i < 8; i++){
        kterm_put(rsd->signature[i]);
    }
    kterm_put('\n');

    printf("OEMID: ");
    for(int i =0; i < 6 ; i++){
        kterm_put (rsd->OEMID[i]);
    }
    kterm_put('\n');

    printf("Revision: %d\n", rsd->Revision);
    printf("RSDT Address: 0x%x\n", rsd->RsdtAddress );
}

RSDPTR* FindRSD(){
    char* memory_byte = (char*) 0xC00f2e14;
    const void* string = "RSD PTR ";

    for( ; (uint32_t) memory_byte < 0xC0100000; memory_byte+=10){
        if( memcmp(memory_byte , string , 8 ) ==  0 ) {
        printf("RSD PTR found at 0x%x !\n", memory_byte);
        break;
        }
    }

    return (RSDPTR*) memory_byte;
}


RSDT* getRSDT(RSDPTR* rsd){

        RSDT* rsdt = (RSDT*) rsd->RsdtAddress;

        printf("OEMID: ");
        for(int i = 0; i < 6; i++){
            kterm_put(rsdt->header.OEMID[i]);
        }
        kterm_put('\n');
        return rsdt;
}