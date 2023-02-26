//
// Created by nigel on 21/02/23.
//
#include "FAT.h"
#include "../../ata pio/ATAPIO.h"
#include "../../../memory/KernelHeap.h"
#include "../../partitiontables/mbr/MasterBootRecord.h"
#include "../../../../CoreLib/ctype.h"
#include "../../../../CoreLib/Memory.h"
#include <CoreLib/Memory.h>
#include <CoreLib/ctype.h>

// exposed driver API
FS_SUPER* FAT::Mount(filesystem *fs, const char* name , vfsmount *mnt)
{

    if( strncmp (fs->name, "fat", 3 ) != 0 )
    {
        printf("Can't mount filesystem with none fat type!\n");
        return nullptr;
    }
    auto* bpb =  GetBiosParameterBlock();
    auto fat_type = DetermineFATType(bpb);

    if(fat_type != FAT_TYPE::FAT16)
        return nullptr;

    FS_SUPER* sb = (FS_SUPER*) malloc(sizeof(FS_SUPER));
    DirectoryNode* root = (DirectoryNode*) malloc(sizeof (DirectoryNode));
    inode* node = (inode*) malloc(sizeof(inode));

    root->children = nullptr;
    node->internal = (void*)FAT::GetSectorOfRootDirectory(bpb); //sector number;
    node->lookup = FAT::Lookup;
    root->name = (char*) name;
    root->node = node;
    root->parent = nullptr;
    root->compare = FAT::Compare;
    mnt->mnt_count =1;
    mnt->mnt_devname = "QEMU HDD";
    mnt->mnt_flags = 0;
    mnt->mnt_parent = nullptr;
    mnt->root = root;
    mnt->sb = sb;
    sb->type = fs;
    sb->root = root;
    sb->fs_info = bpb;

    return sb;
}
FILE FAT::Open(char* filename){

    return (FILE){nullptr, 0, nullptr, nullptr, 1} ;
}
int FAT::Read(FILE* file, void* buffer , unsigned int length)
{

    if(file == nullptr)
    {
        printf("NO FILE!!\n");
        return -1;
    }
    inode* node = file->root;

    if(node== nullptr)
    {
        printf("No INODE!\n");
        return -1;
    }

    int cluster = (int)node->internal;
    auto* bpb = FAT::GetBiosParameterBlock();

    unsigned int FAT_entry = FAT::GetFATEntry(bpb, cluster);
    unsigned int root_dir_sector = FAT::RootDirSize(bpb);
    unsigned int fat_size = bpb->FATSz16;
    unsigned int first_data_sector = bpb->RsvdSecCnt + (bpb->NumFATs * fat_size) + root_dir_sector;
    unsigned int file_data_sector = ((cluster - 2) * bpb->SecPerClus) + first_data_sector;

    ATAPIO::Read(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER, file_data_sector, (uint16_t*)buffer);

    return 0;
}

int FAT::Write(FILE* file, const void* buffer, unsigned int length)
{
    return 0;
}

int FAT::Compare (DirectoryNode*, char* filename, char* filename2)
{
    //TODO Implement proper compare method for 8.3 filenames
   // printf("COMPARE: %s with %s\n", filename, filename2);
    return memcmp(filename, filename2, 11);
}

int FAT::Create(inode* dir_node, inode** target, const char* component_name){}

DirectoryNode* FAT::Lookup (inode* currentDir , DirectoryNode*  dir)
{

    uint16_t data[256];
    ATAPIO::Read(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER, (int)currentDir->internal , data);

    List* lastAdded = nullptr;
    auto* directory = (DIR*)data;
    for(int i = 0; i < sizeof(data) / sizeof (DIR); i++)
    {
        DIR* entry = (DIR*)((uint32_t)directory + (i * sizeof(DIR)));
        if(
            entry->Name[0] == FAT::FREE_DIR ||
            entry->Name[0] == FAT::FREE_DIR_2 ||
            entry->Name[0] == 0xE5 ||
            entry->ATTR & FAT::ATTRIBUTES::ATTR_VOLUME_ID ||
            entry->ATTR & FAT::ATTRIBUTES::ATTR_SYSTEM ||
            entry->ATTR & FAT::ATTRIBUTES::ATTR_HIDDEN
        ){
            continue;
        }

        auto* dirNode = (DirectoryNode*) malloc(sizeof (DirectoryNode));

        char* name = (char*)malloc(sizeof(char[11]));
        memcpy(name, entry->Name, 11 );
        dirNode->name = name;
        dirNode->compare = dir->compare;
        dirNode->parent = dir;

        dirNode->node= (inode*) malloc(sizeof (inode));
        dirNode->node->internal = (void*)entry->FstClusLo;

        dirNode->node->read = currentDir->read;
        dirNode->node->lookup = currentDir->lookup;
        dirNode->node->sb = currentDir->sb;
        dirNode->node->size = entry->FileSize;

        List* dirlist = (List*) malloc(sizeof (List));
        dirlist->data = dirNode;
        dirlist->next = nullptr;

        lastAdded = dirlist;
        auto* temp = dir->children;
        dir->children = lastAdded;
        lastAdded->next = temp;


    }

    return (DirectoryNode*)dir;
}

// internal functions
FAT_TYPE FAT::DetermineFATType(BiosParameterBlock* bpb){
    int RootDirSector = ((bpb->RootEntCnt * 32) + (bpb->BytsPerSec -1)) / bpb->BytsPerSec;
    int FATSz = 0;
    if(bpb->FATSz16 != 0){
        FATSz = bpb->FATSz16;
    } else{
       // FATSz = bpb->FATSz32;

    }
    int TotSec = 0;
    if(bpb->TotSec16 != 0){
        TotSec= bpb->TotSec16;
    }else{
        TotSec = bpb->TotSec32;
    }

    int DataSec = TotSec - (bpb->RsvdSecCnt + (bpb->NumFATs * FATSz) + RootDirSector);
    int CountofClusters = DataSec / bpb->SecPerClus;

    if(CountofClusters < 4085){
        return FAT_TYPE::FAT12;
    } else if (CountofClusters < 65525) {
        return FAT_TYPE::FAT16;
    } else{
        return FAT_TYPE::FAT32;
    }
};
BiosParameterBlock* FAT::GetBiosParameterBlock(bool DEBUG  ){
    BiosParameterBlock* bpb = (BiosParameterBlock*) malloc(sizeof(BiosParameterBlock));
    uint16_t StartAddress = 0x00 ;
    ATAPIO::Read(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER, StartAddress, (uint16_t*) bpb);

    if(DEBUG)
    {
        printf("OEM ID: %s\n", bpb->OEMName);
        printf("Bytes per sector: %d\n", bpb->BytsPerSec);
        printf("Sectors per cluster: %d\n", bpb->SecPerClus);
        printf("Reserved sectors: %d\n", bpb->RsvdSecCnt);
        printf("Number of FAT: %d\n", bpb->NumFATs);
        printf("Number of Dir entries: %d\n", bpb->RootEntCnt);
        printf("Total Sectors in volume: %d\n", bpb->TotSec16 == 0 ? bpb->TotSec32 : bpb->TotSec16);
        printf("Sectors per FAT: %d\n", bpb->FATSz16 );
    }

    return bpb;
}
uint16_t FAT::GetFATEntry (BiosParameterBlock* bpb, unsigned int cluster){
    int FATSz = bpb->FATSz16;
    int FATOffset = 0;
    FAT_TYPE type = FAT::DetermineFATType(bpb);
    if( type == FAT_TYPE::FAT16){
        FATOffset = cluster *2;
    } else if( type == FAT_TYPE::FAT32){
        FATOffset = cluster * 4;
    }
    int thisFATSecNum = bpb->RsvdSecCnt + (FATOffset / bpb->BytsPerSec); // Sector number containing the entry for the cluster

    // For any other FAT other than the default
    // SectorNumber = (FATNumber * FATSz) + ThisFATSecNum

    uint16_t buff[bpb->BytsPerSec];
    ATAPIO::Read(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER, thisFATSecNum, buff );
    int thisFATEntOffset = FATOffset % bpb->BytsPerSec; // offset  for the entry in the sector containing the entry for the cluster
    uint16_t ClusterEntryValue = 0;
    // Get the FATEntry
    if(type == FAT_TYPE::FAT16){
        return *((uint16_t*) &buff[thisFATEntOffset]);

    }
    else{
        // FAT32 logic
        return 0;
    }

}
uint16_t FAT::DetermineFreeSpace()
{
    // Loop through all FAT entries in all FAT's
    // to construct a list of free/available clusters
    // Free clusters are recorded with all 0's except on FAT32 where
    // the highest order 4 bits should be ignored.

/*
 * The number of sectors reserved for each FAT (count of sectors in the BPB_FATSz16 or
BPB_FATSz32 fields) may be bigger than the actual number of sectors required for
containing the entire FAT. Therefore, there may be totally unused FAT sectors at the end of
each FAT in the FAT region of the volume. Each implementation must determine the value
for the last valid sector in the FAT using CountOfClusters (the last valid sector in the FAT
is the one containing the FAT entry numbered CountOfClusters + 1).
All sectors reserved for the FAT beyond the last valid sector (defined as the one containing
the FAT entry for the last cluster) must be set to 0x0 during volume initialization/format.
 */


}
int FAT::GetSectorOfRootDirectory (BiosParameterBlock* bpb)
{
   return (bpb->RsvdSecCnt + (bpb->NumFATs * bpb->FATSz16));
}
unsigned int FAT::RootDirSize(BiosParameterBlock* bpb)
{
    return ((bpb->RootEntCnt * 32) + (bpb->BytsPerSec -1)) /bpb->BytsPerSec;

}
void FAT::OpenSubdir(DIR* directory, BiosParameterBlock* bpb ){

    unsigned int cluster = directory->FstClusLo;
    printf("Listing contents of " );

     for(unsigned char n : directory->Name){
         if(n == 0x20)
             continue;
         kterm_put(n);
     }
    kterm_put('\n');
    printf("FsCluHi: 0x%x , FsCluLo: 0x%x\n", directory->FstClusHi, directory->FstClusLo);
    printf("Cluster: 0x%x\n", cluster);
    unsigned int FATEntry = FAT::GetFATEntry(bpb, cluster);
    printf("FAT_Entry: 0x%x\n", FATEntry);

    unsigned int root_dir_sectors = FAT::RootDirSize(bpb);
    unsigned int fat_size = bpb->FATSz16;
    unsigned int first_data_sector = bpb->RsvdSecCnt + ( bpb->NumFATs * fat_size) + root_dir_sectors;
    unsigned int first_directory_sector = ((cluster - 2) * bpb->SecPerClus) + first_data_sector;
    printf("Directory first sector 0x%x\n" , first_directory_sector);
    uint16_t data[256];
    ATAPIO::Read(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER, first_directory_sector, data);

    auto* directoryContents = (DIR*) data;
    for(int i = 0; i < sizeof(data) / sizeof(DIR); i++){
        DIR* entry = (DIR*)((uint32_t)directoryContents + (i * sizeof(DIR)));

        if(entry->Name[0] == FAT::FREE_DIR || entry->Name[0] == FAT::FREE_DIR_2 || entry->Name[0] == 0xE5)
            continue;

        if(entry->ATTR & FAT::ATTRIBUTES::ATTR_HIDDEN){
            continue;
        }

        if(entry->ATTR & FAT::ATTRIBUTES::ATTR_SYSTEM)
            continue;


        if(entry->ATTR & FAT::ATTRIBUTES::ATTR_VOLUME_ID){
            continue;
        }

        if (!(entry->ATTR & FAT::ATTRIBUTES::ATTR_LONG_NAME)){
            for(char n : entry->Name){
                if(n == 0x20)
                    continue;
                kterm_put(n);
            }
            kterm_put('\n');
        }else{
            printf("LFN\n");
        }

    }

}
void FAT::ReadFileContents(DIR* fileEntry , BiosParameterBlock* bpb){
    unsigned int cluster = fileEntry->FstClusLo;
    printf("cluster NR: %x\n", cluster);
    unsigned int FATEntry = FAT::GetFATEntry(bpb, cluster);
    unsigned int root_dir_sectors = FAT::RootDirSize(bpb);
    unsigned int fat_size = bpb->FATSz16;
    unsigned int first_data_sector = bpb->RsvdSecCnt + (bpb->NumFATs * fat_size) + root_dir_sectors;
    unsigned int file_data_sector = ((cluster -2) * bpb->SecPerClus) + first_data_sector;
    printf("FAT entry = %x\n", FATEntry);
    uint16_t data[256];
    ATAPIO::Read(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER, file_data_sector, data);

    for (unsigned short n : data)
    {
        kterm_put(n & 0x00ff);
        kterm_put(n >> 8);
    }
    kterm_put('\n');

}
void FAT::ListRootDirectoryContents(BiosParameterBlock* bpb){
    int total_sectors = bpb->TotSec32;
    int fat_size = bpb->FATSz16;
    int root_dir_sectors = FAT::RootDirSize(bpb);
    int first_data_sector = bpb->RsvdSecCnt + (bpb->NumFATs * fat_size) + root_dir_sectors ;
    int data_sectors = bpb->TotSec32 - (bpb->RsvdSecCnt + (bpb->NumFATs * fat_size) + root_dir_sectors);
    int total_clusters = data_sectors / bpb->SecPerClus;


    int first_root_dir_sector = first_data_sector - root_dir_sectors;
    //int first_sector_of_cluster = ((cluster - 2) * bpb->SecPerClus) + first_data_sector;
    uint16_t data[256];
    ATAPIO::Read(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER, first_root_dir_sector, data);

    auto* RootDirectory = (DIR*)data;
    for(int i = 0; i < sizeof(data) / sizeof (DIR); i++)
    {
        DIR* entry = (DIR*)((uint32_t)RootDirectory + (i * sizeof(DIR)));


        if(entry->Name[0] == FAT::FREE_DIR || entry->Name[0] == FAT::FREE_DIR_2 || entry->Name[0] == 0xE5){
            continue;
        }


        if(entry->ATTR & FAT::ATTRIBUTES::ATTR_HIDDEN){
            continue;
        }

        if(entry->ATTR & FAT::ATTRIBUTES::ATTR_SYSTEM)
            continue;


        if(entry->ATTR & FAT::ATTRIBUTES::ATTR_VOLUME_ID){
            continue;
        }
        if (!(entry->ATTR & FAT::ATTRIBUTES::ATTR_LONG_NAME)){
            for(char n : entry->Name){
                if(n == 0x20)
                    continue;
                kterm_put(n);
            }
        }else{
            printf("Long file name detected!");
        }

        printf(" [Size: %d bytes, Attributes: %d]\n", entry->ATTR, entry->FileSize);
        if(entry->ATTR & FAT::ATTRIBUTES::ATTR_DIRECTORY ){
            FAT::OpenSubdir(entry, bpb);
        } else {
            FAT::ReadFileContents(entry, bpb);
        }

    }
}


