#pragma once 
#include <stdint.h>
#include "PartitionTableEntry.h"

struct MBR {
    uint8_t code [440];
    uint32_t uniqueID;
    uint16_t Reserved;
    PartitionTableEntry TableEntries[4];
    uint16_t ValidBootsector;
}__attribute__((packed));