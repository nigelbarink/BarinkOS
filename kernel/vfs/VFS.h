#pragma once
#include <stdint.h>
#include "../../CoreLib/Path.h"

#define FS_FILE 0
#define FS_DIRECTORY 1
#define FS_INVALID 2
#define DEVICE_MAX 26

typedef struct _FILE {
    char name [32];
    uint32_t flags;
    uint32_t filelength;
    uint32_t id;
    uint32_t eof;
    uint32_t position;
    uint32_t currentCluster;
    uint32_t device;
}FILE, *PFILE;

typedef struct _FILE_SYSTEM{
    char name[8];
    FILE (*Directory) (const char* Directoryname);
    void (*Mount) ();
    void (*Read) (PFILE file, unsigned char* buffer, unsigned int length);
    void (Close) (PFILE);
    FILE (*Open) (const char* filename);
}FILESYSTEM, *PFILESYSTEM;

typedef struct _MOUNT_INFO{
    uint32_t numSectors;
    uint32_t fatOffset;
    uint32_t numRootEntries;
    uint32_t rootOffset;
    uint32_t rootSize;
    uint32_t fatSize;
    uint32_t fatEntrySize;
}MOUNT_INFO, *PMOUNT_INFO;

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
}DIRECTORY, *PDIRECTORY;
// Date Format
// [0..4] Day
// [5..8] Month
// [9..15] Year

// Time Format
// [0..4] Seconds
// [5..10] Minute
// [11..15] Hour
extern PFILESYSTEM _filesystems[DEVICE_MAX];

 FILE volOpenFile(const char* fname);

 void volCloseFile(PFILE file);
 void volRegisterFilesystem(PFILESYSTEM, unsigned int deviceID);
 void volUnregisterFilesystem(PFILESYSTEM);
 void volUnregisterFileSystemByID(unsigned int deviceID);


 class FileSystem{
 public:
     static void initialize();

     static void ResolvePath(Path& path);

 };