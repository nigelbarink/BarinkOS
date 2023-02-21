//
// Created by nigel on 21/02/23.
//
#pragma once
#include <stdint-gcc.h>

enum FS_TYPES {
    FS_FILE =0,
    FS_DIRECTORY =1,
    FS_INVALID=2
};

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
    void (*Write)(PFILE file, unsigned char* buffer, unsigned int length);
    void (*Close) (PFILE);
    FILE (*Open) (char* filename);
}FILESYSTEM, *PFS;

typedef struct _PARTITION {
    uint32_t Disk;
    uint32_t StartAddress;
    uint32_t Sectors;
    uint8_t Fs_hint;
    uint8_t Attributes;
}PARTITION, *PTR_PARTITION;
