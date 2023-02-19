#pragma once 
#include <stdint.h>

struct DirectoryEntry {
    uint8_t filename [8];
    uint8_t Extension [3];
    uint8_t attribute;
    uint8_t Reserved;
    uint8_t creation;
    uint16_t CreationTime;
    uint16_t CreationDate;
    uint16_t LastAccessDate;
    uint16_t ReservedFAT32;
    uint16_t LastWriteTime;
    uint16_t LastWriteDate;
    uint16_t StartingCluster;
    uint32_t FilesizeInBytes;

}__attribute__((packed));