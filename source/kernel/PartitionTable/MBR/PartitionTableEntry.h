#pragma once
#include <stdint.h>

struct PartitionTableEntry{
    uint8_t driveAttribute;
    uint8_t CHS_start_address [3];
    uint8_t PartitionType;
    uint8_t CHS_lastSector_Address[3];
    uint32_t LBA_partition_start;
    uint32_t Number_sectors_inPartition;
}__attribute__((packed));