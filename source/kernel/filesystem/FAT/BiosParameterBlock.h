#pragma once
#include <stdint.h>
#include "./ExtendBootRecord.h"

struct BiosParameterBlock {
    uint8_t BootLoaderCodeSection [3];
    uint8_t OEM_id [8];
    uint16_t  BytesPerSector ; // I suspect would be 512
    uint8_t SectorsPerCluster ; 
    uint16_t ReservedSectors;
    uint8_t NumberOfFileAllocationTables; // Probably equals 2
    uint16_t NumberOfDirectoryEntries; // Root directory must contain entire sectors
    uint16_t TotalSectorsInLogicalVolume ; // 0 means >65535 sectors in volume , actual count can be found in LargeSectorCount
    uint8_t MediaDescriptor ; // Indication the media descriptor type
    uint16_t NumberOfSectorsPerFAT;// only in FAT12 / FAT 16
    uint16_t NumberOfSectorsPerTrack; 
    uint16_t NumberOfHeadsOnMedia;
    uint32_t NumberOfHiddenSectors;
    uint32_t LargeSectorCount;
    ExtendedBootRecord_FAT16 ebpb;
}__attribute__((packed));