#pragma once
#include <stdint.h>
#include "./../../terminal/kterm.h"
#include "../../../lib/include/mem.h"
struct RSDPTR {
    char signature[8];
    uint8_t Checksum ;
    char OEMID [6];
    uint8_t Revision;
    uint32_t RsdtAddress;
}__attribute__((packed));

struct ACPISDTHeader{
    char Signature[4];
    uint32_t Length;
    uint8_t CheckSum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
}__attribute__((packed));


struct RSDT{
    struct ACPISDTHeader header;
    uint32_t PointerToSDT[]; // Length of array : (header.Length - sizeof(header))/ 4
}__attribute__((packed));


//NOTE: only scans EBDA enough to find RSD PTR in QEMU
RSDPTR* FindRSD();

void printRSD(RSDPTR* rsd);

RSDT* getRSDT(RSDPTR* rsd);