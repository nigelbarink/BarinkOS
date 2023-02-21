#pragma once 
#include <stdint-gcc.h>
#include "PartitionTableEntry.h"
#include "../../../memory/KernelHeap.h"
#include "../../ata pio/ATAPIO.h"

struct MBR {
    uint8_t code [440];
    uint32_t uniqueID;
    uint16_t Reserved;
    struct PartitionTableEntry TableEntries[4];
    uint16_t ValidBootsector;
}__attribute__((packed));


inline MBR* GetPartitions(bool DEBUG = false){
    const int C = 0;
    const int H = 0;
    const int HPC = 16;
    const int SPT = 63;

    int S =1;
    uint32_t LBA = (C*HPC+H) * SPT + (S-1);

    MBR* mbr =(MBR*) malloc(sizeof (MBR));

    ATAPIO::Read(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER, LBA, (uint16_t*)mbr);


    printf("MBR (In Memory) Address 0x%x, Size = %d\n", mbr, sizeof (MBR));
    if(DEBUG){
        printf("BootSector: 0x%x\n", mbr->ValidBootsector );
        for( int i = 0 ; i < 4 ; i ++){
            PartitionTableEntry PT = mbr->TableEntries[i];

            printf("Partition %d [  %d sectors,  PartitionType: 0x%x, 0x%x, \nLBA Start: 0x%x ]\n" ,
                   i, PT.Number_sectors_inPartition, PT.PartitionType, mbr->uniqueID,  PT.LBA_partition_start );
        }

    }

    return mbr;
}
