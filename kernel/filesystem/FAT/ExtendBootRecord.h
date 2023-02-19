#pragma once 
#include <stdint.h>

struct ExtendedBootRecord_FAT16{
    uint8_t DriveNumber;
    uint8_t Reserved;
    uint8_t Signature;
    const uint32_t VOLUME_ID_SERIAL_NUMBER;
    uint8_t volume_label [11];
    uint8_t Identifier_string [8];
    uint8_t bootCode [448];
    uint16_t partitionSignature;
}__attribute__((packed));

struct ExtendedBootRecord_FAT32{
    uint32_t SectorsPerFAT;
    uint16_t Flags;
    const uint16_t FAT_VERSION_NUMBER;
    uint32_t rootDirectory_clusterNumber;// Often set to 2;
    uint16_t FSInfo_SectorNumber;
    uint16_t backup_bpb_sectorNumber;
    uint8_t Reserved [12];
    uint8_t DriveNumber;
    uint8_t Reserved2;
    uint8_t Signature; // must be 0x28 or 0x29 
    uint32_t VOLUME_ID_SERIAL;
    uint8_t volume_label[11];
    uint8_t SystemIdentifierString [8]; // ALWAYS "FAT32 " but spec says do not trust
    uint8_t BootCode [420]; // NICE
    uint16_t PartitionSignature; // 0xAA55

}__attribute__((packed));