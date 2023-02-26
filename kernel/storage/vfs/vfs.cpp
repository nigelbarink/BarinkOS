#include "vfs.h"
#include <stdint-gcc.h>
#include "../../memory/KernelHeap.h"
#include "../ata pio/ATAPIO.h"
#include "../partitiontables/mbr/MasterBootRecord.h"
#include "../filesystems/FAT/FAT.h"
#include "vfs_types.h"
#include "../../../CoreLib/Memory.h"
#include <CoreLib/Memory.h>

vfsmount* VirtualFileSystem::rootfs;
int VirtualFileSystem::mount_number = 0;
int VirtualFileSystem::superblock_number =0;
int VirtualFileSystem::filesystem_number =0;

filesystem* VirtualFileSystem::filesystems[4];
FS_SUPER* VirtualFileSystem::superblocks[8];
vfsmount* VirtualFileSystem::mounts[12];

void VirtualFileSystem::Mount(filesystem* fs, const char* name)
{

    vfsmount* mnt_point = (vfsmount*) malloc(sizeof(vfsmount));
    FS_SUPER* sb = fs->mount(fs, name, mnt_point);
    if( sb == nullptr){
        printf("mount failed!\n");
        return;
    }
    mounts[mount_number++] = mnt_point;
    superblocks[superblock_number++] = sb;
    mnt_point->mnt_count = 1;
    rootfs = mnt_point;
}

void VirtualFileSystem::initialize()
{
    // TODO: setup memory pools etc to speed things up
    // a bit
    // TODO: Add a devfs, procfs etc...

    filesystem* fat_fs = (filesystem*) malloc(sizeof (filesystem));
    fat_fs->name = "fat";
    fat_fs->mount = FAT::Mount;
    //register_filesystem(fat_fs);


    // Mount the bootdrive
    // NOTE: for now will hardcode this
    Mount(fat_fs, "/");
};


int VirtualFileSystem::register_filesystem(struct filesystem* fs) {
    // register the filesystem to the kernel.
    filesystems[filesystem_number] = fs;
    filesystem_number++;


}

FILE* VirtualFileSystem::open(const char* pathname, int flags){
    // 1. Lookup pathname from the root node
    // 2. Create a new file descriptor for this inode if found.
    // 3. Create a new file if O_CREATE is specified in the flags.
    FILE* file = (FILE*) malloc(sizeof (FILE)) ;
    auto* rootentry = rootfs->root;
    char* tokstate = nullptr;
    auto nextdir = strtok ((char*)pathname, "/", &tokstate );

    // look up children if not filled
    if(rootentry->children == nullptr)
    {
        rootentry = rootentry->node->lookup(rootentry->node, rootentry);
    }

    if(rootentry->children == nullptr)
    {
        file->flags =1;
        return file;
    }

    // let's just loop through the folder first
    auto* child = rootentry->children;
    while(child->next != nullptr){

        auto* directory = (DirectoryNode*)child->data;
        if( directory->compare(directory, directory->name, nextdir) == 0){
            nextdir = strtok(nullptr, "/", &tokstate);
            if(nextdir == NULL){
                file->root = directory->node;
                file->flags =0;
                file->read = FAT::Read;
                return file;
            }
        }
        child = child->next;
    }



    file->flags = 1;
    return file;
}

int VirtualFileSystem::close (struct FILE* file){
    // 1. release the file descriptor
}
int VirtualFileSystem::write(struct FILE* file, const void* buf, unsigned int len){
    // 1. Write len bytes from buf to the opened file.
    // 2. return written size or error code if an error occurs
}
int VirtualFileSystem::read (struct FILE*  file, void* buf, unsigned int len){
    // 1. read min(len, readable file data size) bytes ro buf from the opened file.
    // 2. return read size or error code if an error occurs
}

/*

void fs_discovery(){


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
        }


}



*/