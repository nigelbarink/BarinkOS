//
// Created by nigel on 21/02/23.
//
#pragma once
#include "../../vfs/vfs_types.h"
#include "../../vfs/vfs_types.h"
#include "../../partitiontables/mbr/MasterBootRecord.h"

struct ExtendedBootRecord_FAT16{
    uint8_t DrvNum;
    uint8_t Reserved1;
    uint8_t BootSig;
    const uint32_t VolID;
    uint8_t VolLab [11];
    uint8_t FilSysType [8];
    uint8_t bootCode [448];
    uint16_t Signature_word;
    uint8_t SecRmndr[512];
}__attribute__((packed));

struct BiosParameterBlock {
    uint8_t jmpBoot[3];
    uint8_t OEMName [8];
    uint16_t  BytsPerSec ; // I suspect would be 512
    uint8_t SecPerClus ;
    uint16_t RsvdSecCnt;
    uint8_t NumFATs; // Probably equals 2
    uint16_t RootEntCnt; // Root directory must contain entire sectors
    uint16_t TotSec16 ; // 0 means >65535 sectors in volume , actual count can be found in LargeSectorCount
    uint8_t Media ; // Indication the media descriptor type
    uint16_t FATSz16;// only in FAT12 / FAT 16
    uint16_t SecPerTrk;
    uint16_t NumHeads;
    uint32_t HiddSec;
    uint32_t TotSec32;
    ExtendedBootRecord_FAT16 ebpb;
}__attribute__((packed));

struct DIR {
    uint8_t Name [11];
    uint8_t ATTR ;
    uint8_t NTRes;
    uint8_t CrtTimeTenth; // File Creation time component - count of tenths of a second (between 0 and 199)
    uint16_t CrtTime; // Creation time. Granularity is 2 seconds
    uint16_t CrtDate; // Creation date.
    uint16_t LstAccDate; // Last Access Date (Last read or write date)
    uint16_t FstClusHi; // High Word of first data cluster for file/directory described
    uint16_t WrtTime; // Last Modification time | Must equal CrtTime
    uint16_t WrtDate; // Last Modification date |  Must equal CrtDate
    uint16_t FstClusLo; // Low word of first data cluster for file/directory described
    uint32_t FileSize; // size in bytes of file/directory described
}__attribute__((packed));


enum struct FAT_TYPE{
    FAT12,
    FAT16,
    FAT32,
    VFAT,
    UNKOWN
};

class FAT {
public:


    // Wanted API for vfs
    static file Open(char* filename);
    static int close(file* file);
    static int Read(file* file, void* buffer , int length);
    static int Write(file* file, const void* buffer, int length);
    static int create(inode* dir_node, inode** target, const char* component_name);
    static int lookup(inode* , inode**, const char*);
    static int compare(directoryEntry* , char* , char*);
    static superblock* Mount(filesystem* fs, const char* name ,vfsmount* mount);

    // TEMP
    static void listFilesInRoot(MBR* mbr, BiosParameterBlock* bpb );
    static BiosParameterBlock* getBPB( bool DEBUG =false );
    static FAT_TYPE determineFATType(BiosParameterBlock* bpb);
    static uint16_t GetFATEntry(BiosParameterBlock*, unsigned int);
    static uint16_t DetermineFreeSpace();
    static int GetSectorOfRootDirectory(BiosParameterBlock*);
    static unsigned int RootDirSize(BiosParameterBlock*);
    static void OpenSubdir (DIR*, BiosParameterBlock*);



    static const int  FREE = 0x0000;
    static const int  ALLOCATED = 0x0002;
    static const int BAD = 0xFFF7;
    static const int EOF = 0xFFFF;

    static const int ClnShutBitMask = 0x8000;
    static const int HrdErrBitMask = 0x4000;

    static const char DOS_TRAILING_SPACE = 0x20;
    static const char FREE_DIR = 0xE5; // If KANJI charset 0x05
    static const char FREE_DIR_2 = 0x00; // All directories after this are free including this one

    enum ATTRIBUTES {
        ATTR_READ_ONLY = 0x01,
        ATTR_HIDDEN = 0x02,
        ATTR_SYSTEM = 0x04,
        ATTR_VOLUME_ID = 0x08,
        ATTR_DIRECTORY = 0x10,
        ATTR_ARCHIVE = 0x20,
        ATTR_LONG_NAME = (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)
    };

private:


    enum ENTRY_SIZE {
        FAT12 = 12,
        FAT16 = 16,
        FAT32 = 32
    };



};
