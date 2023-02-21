#pragma once 
#include <stdint-gcc.h>

struct DirectoryEntry {
    uint8_t filename [8];
    uint8_t Extension [3];
    uint8_t attribute;
    uint8_t Reserved;
    uint8_t creation; // Creation in tenths of a second
    uint16_t CreationTime; // Time Created NOTE: Multiply the seconds by 2
    uint16_t CreationDate; // Date Created
    uint16_t LastAccessDate;
    uint16_t ReservedFAT32;
    uint16_t LastWriteTime;
    uint16_t LastWriteDate;
    uint16_t StartingCluster;
    uint32_t FilesizeInBytes;

}__attribute__((packed));



typedef struct _DIRECTORY{
    uint8_t Filename[8];
    uint8_t Ext[3];
    uint8_t Attrib;
    uint8_t Reserved;
    uint8_t TimeCreatedMs;
    uint16_t TimeCreated;
    uint16_t DateCreated;
    uint16_t DateLastAccessed;
    uint16_t FirstClusterHiBytes;
    uint16_t LastModTime;
    uint16_t LastModDate;
    uint16_t FirstCluster;
    uint32_t FileSize;
}__attribute__((packed)) DIRECTORY, *PDIRECTORY;