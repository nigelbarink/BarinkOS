#include "vfs.h"
#include <stdint-gcc.h>
#include "../../memory/KernelHeap.h"
#include "../ata pio/ATAPIO.h"
#include "../partitiontables/mbr/MasterBootRecord.h"
#include "../filesystems/FAT/FAT.h"
#include "StorageTypes.h"

#include <CoreLib/Memory.h>

PFS VirtualFileSystem::_filesystems[VirtualFileSystem::DEVICE_MAX];
PTR_PARTITION VirtualFileSystem::_partitions [VirtualFileSystem::PARTITION_MAX];
unsigned int VirtualFileSystem::num_partitions = 0;

void VirtualFileSystem::initialize()
{

    // Mount the boot disk
    // NOTE: we assume for now that it is the only disk in the system
    //      This information could possibly be had from the bootloader (GRUB)
    // We als assume it is the primary device on the Master port.
    ATAPIO::Soft_Reset(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER);
    bool isAvailable = ATAPIO::Identify(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER);
    if(!isAvailable){
        // PANIC!!!
        printf("Failed to mount root filesystem!\n");
        return;
    }

    auto masterbootrecord = GetPartitions(false);

    for (auto partition : masterbootrecord->TableEntries)
    {
        if(partition.PartitionType == 0x0) continue; // Consider marked as free

        PTR_PARTITION found_partition = (PARTITION*) malloc(sizeof(PARTITION));
        found_partition->Disk = ATAPIO_PORT::Primary | (  DEVICE_DRIVE::MASTER << 16);
        printf("Disk Identifier: 0x%x\n", found_partition->Disk);
        found_partition->Attributes = partition.driveAttribute;
        found_partition->StartAddress = partition.LBA_partition_start;
        found_partition->Sectors = partition.Number_sectors_inPartition;
        found_partition->Fs_hint = partition.PartitionType;

        VirtualFileSystem::RegisterPartition(found_partition);
    }

    printf("Found %d partitions on disk!\n", num_partitions);

    for (int i = 0; i < num_partitions; i++)
    {
        auto* partition = _partitions[i];
        // Check the fs_hint for a proper driver
        if ( partition->Fs_hint != 0x06){
            printf("Assumed Unkown filesystem!\n");
            continue;
        }
        // Check if filesystem OK

        printf("Partition Start Address (LBA): 0x%x\n", partition->StartAddress);
        bool valid = FAT::Validate(partition);
        if(!valid)
        {
            printf("Not a valid FAT fs!\n");
            continue;
        }

        // setup FileSystem Description before mounting
        PFS  FS_FAT = (PFS)malloc(sizeof(FILESYSTEM));

        FAT::Info(partition, FS_FAT);

        // Mount the partition/filesystem
        Mount(FS_FAT , i);
    }




};

void VirtualFileSystem::RegisterPartition(PTR_PARTITION partition) {
    _partitions[num_partitions] = partition;
    num_partitions++;
}

FILE VirtualFileSystem::OpenFile(const char* path)
{
    // See reference material (1) https://man7.org/linux/man-pages/man7/path_resolution.7.html
    unsigned char device = 'a';
    char* filename = (char*)path;
    char* cpy = filename;


    if(filename[1] == ':'){
        device = filename[0];
        filename += 2;
    }


    if ( _filesystems[device - 'a']){
        // Unfortunately this way the FAT Driver doesn't know which device and which partition to read from
        // leaving us hopeless of finding the file.
        FILE file = _filesystems[device-'a']->Open(filename);
        file.device = device;
        free(cpy);
        return file;
    }

    free(cpy);
    FILE  file;
    file.flags = FS_INVALID;
    return file;
}

void VirtualFileSystem::Mount(PFS filesystemDescriptor, unsigned int DeviceID)
{
    if(DeviceID < DEVICE_MAX)
        if(filesystemDescriptor)
            _filesystems[DeviceID] = filesystemDescriptor;
}

void VirtualFileSystem::Unmount(unsigned int DeviceID) {
    if(DeviceID < DEVICE_MAX)
        _filesystems[DeviceID] = nullptr;
}





