#pragma once
#include <stdint.h>
struct SuperBlock {  
    uint32_t NumberOfInodes;
    uint32_t NumberOfBlocks;
    uint32_t NumberOfReservedBlocks;
    uint32_t NumberOfUnallocatedBlocks;
    uint32_t NumberOfUnallocatedInodes;
    uint32_t BlockNumberOfSuperBlock;
    uint32_t BlockSize;// Something about a shift left 
    uint32_t FragmentSize;
    uint32_t NumberOfBlocksInGroup;
    uint32_t NumberOfFragmentsInBlockGroup;
    uint32_t NumberOfInodesInBlockGroup;
    uint32_t LastMountTime; // POSIX
    uint32_t LastWrittenTime; // POSIX
    uint16_t TimesMountedSinceCheck; 
    uint16_t TimesMountedUntilCheck;
    uint16_t EXT_SIG ; // 0xef53
    uint16_t FS_STATE;
    uint16_t ON_ERR;
    uint16_t VERSION_MINOR;
    uint32_t TimeLastCheck; // POSIX
    uint32_t CheckInterval; //POSIX
    uint32_t OS_ID; // OS the FS was created with
    uint32_t VERSION_MAJOR;
    uint16_t UIDReservedBlocks; 
    uint16_t GIDReservedBlocks;
}__attribute__((packed));