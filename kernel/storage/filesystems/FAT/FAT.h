//
// Created by nigel on 21/02/23.
//
#pragma once
#include "ExtendBootRecord.h"
#include "BiosParameterBlock.h"
#include "DirectoryEntry.h"
#include "../../vfs/StorageTypes.h"

// Date Format
// [0..4] Day
// [5..8] Month
// [9..15] Year

// Time Format
// [0..4] Seconds
// [5..10] Minute
// [11..15] Hour


class FAT {
public:



    static bool Validate(PTR_PARTITION partition );

    static FILE Open(char* filename);
    static void Read(PFILE file, unsigned  char* buffer , unsigned int length);
    static void Write(PFILE file, unsigned char* buffer, unsigned int length);

    static void Info(_PARTITION *pPartition, PFS pSystem);

private:
    enum struct TYPE{
        FAT,
        FAT16,
        FAT32,
        VFAT
    };
    enum ATTRIBUTES {
        ATTR_READ_ONLY = 0x01,
        ATT_HIDDEN = 0x02,
        ATTR_SYSTEM = 0x04,
        ATTR_VOLUME_ID = 0x08,
        ATTR_DIRECTORY = 0x10,
        ATTR_ARCHIVE = 0x20
    };
};
