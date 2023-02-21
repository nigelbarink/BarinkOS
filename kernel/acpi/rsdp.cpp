#include "rsdp.h"
#include "../memory/VirtualMemoryManager.h"


void printRSD(RSDPDescriptor* rsd){
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

RSDPDescriptor* FindRSD(){
    char* memory_byte = (char*) 0x000f2e14;
    const void* string = "RSD PTR ";

    for( ; (uint32_t) memory_byte < 0x00100000; memory_byte+=10){
        if( memcmp(memory_byte , string , 8 ) ==  0 ) {
        printf("RSD PTR found at 0x%x !\n", memory_byte);
        break;
        }
    }
    return (RSDPDescriptor*) memory_byte;
}


RSDT* getRSDT(RSDPDescriptor* rsd){
    printf("rsdt Address: 0x%x\n", rsd->RsdtAddress);
    return (RSDT*)rsd->RsdtAddress ;

}