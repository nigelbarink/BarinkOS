#pragma once
#include "../terminal/kterm.h"
#include <CoreLib/Memory.h>
#include <stdint-gcc.h>


struct ACPISDTHeader{
    char Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t CheckSum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
};


struct RSDT{
    struct ACPISDTHeader h;
    uint32_t PointerToSDT[]; // Length of array : (header.Length - sizeof(header))/ 4
}__attribute__((packed));


struct RSDPDescriptor {
    char signature[8];
    uint8_t Checksum ;
    char OEMID [6];
    uint8_t Revision;
    RSDT* RsdtAddress;
}__attribute__((packed));

struct RSCPDescriptor20{
   RSDPDescriptor base;
   uint32_t Length;
   uint64_t XsdtAddress;
   uint8_t ExtendedChecksum;
   uint8_t reserved[3];
}__attribute__((packed));

RSDPDescriptor* FindRSD();

void printRSD(RSDPDescriptor* rsd);

RSDT* getRSDT(RSDPDescriptor* rsd);