//
// Created by nigel on 21/02/23.
//
#include "FAT.h"
#include "../../ata pio/ATAPIO.h"
#include "../../../memory/KernelHeap.h"
#include "../../../../CoreLib/Memory.h"
#include "../../partitiontables/mbr/MasterBootRecord.h"
#include <CoreLib/Memory.h>
superblock* FAT::Mount(filesystem *fs, const char* name ,vfsmount *mnt)
{

    if( strncmp (fs->name, "fat", 3 ) != 0 )
    {
        printf("Can't mount filesystem with none fat type!\n");
        return nullptr;
    }

    superblock* sb = (superblock*) malloc(sizeof(superblock));
    directoryEntry* root = (directoryEntry*) malloc(sizeof (directoryEntry));
    root->name = (char*) name;
    root->node = nullptr;
    root->parent = nullptr;

    dentry_operations* op = (dentry_operations*) malloc(sizeof(dentry_operations));
    op->compare = FAT::compare;


    root->op = op;


    mnt->mnt_count =1;
    mnt->mnt_devname = "QEMU HDD";
    mnt->mnt_flags = 0;
    mnt->mnt_parent = nullptr;

    mnt->root = root;
    mnt->sb = sb;

    sb->type = fs;
    sb->root = root;
    //sb->fs_info = getBPB();

    return sb;
}
int FAT::Read(file* file, void* buffer , int length)
{
    return 0;
}
int FAT::Write(file* file, const void* buffer, int length)
{
    return 0;
}
int FAT::compare (directoryEntry*, char* filename, char* filename2)
{
    // use the size of the smallest string
    int a = strlen(filename);
    int b = strlen(filename2);

    if( a == b ){
        return strncmp(filename, filename2, a);
    }
    return a-b;
}
int FAT::create(inode* dir_node, inode** target, const char* component_name){}
int FAT::lookup (inode*, inode**, const char*){}


FAT_TYPE FAT::determineFATType(BiosParameterBlock* bpb){
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
BiosParameterBlock* FAT::getBPB( bool DEBUG  ){
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
    FAT_TYPE type = FAT::determineFATType(bpb);
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



uint16_t* ReadFAT (BiosParameterBlock& bpb ,  bool DEBUG = false ) {
    uint32_t FATAddress = /*StartAddress*/ 0x00 +  bpb.RsvdSecCnt ;
    uint16_t* FAT = (uint16_t*)malloc(sizeof (uint16_t) * 256);

    ATAPIO::Read(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER, FATAddress, FAT );

    // Show data in terminal
    if(DEBUG){
        for( unsigned int i =0 ; i < 256 ; i++) {
            printf("0x%x ", (unsigned short)FAT[i]);
        }
        kterm_put('\n');
    }

    return FAT;
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


void readFile(uint32_t DataRegion, DIR* entry, uint16_t FATentry, BiosParameterBlock& bpb ){
    printf("Show contents");

    printf("Start cluster of the file: 0x%x\n", entry->FileSize);

    printf("IS it only 1 cluster? %s\n", FATentry == 0xFFFF ? "Yes" : "No");

    uint32_t sector = DataRegion + ((entry->FileSize - 0x02) * bpb.SecPerClus);


    uint16_t dataBlob[256];
    ATAPIO::Read(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER, sector, dataBlob);
    for (unsigned short n: dataBlob) {
        kterm_put(n & 0x00ff);

        kterm_put(n >> 8);
    }
    kterm_put('\n');
}



/*
file fsysFatDirectory (const char* DirectoryName){
    file file;
    unsigned char* buf;
    PDIRECTORY directory;

    char DosFileName[11];
    //ToDosFileName(DirectoryName, DosFileName, 11);
    DosFileName[11] =0;

    for (int sector=0; sector <14 ; sector++){
        ATAPIO::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, mountInfo.rootOffset + sector, (uint16_t*)buf);
        directory = (PDIRECTORY) buf;

        for (int i =0; i < 16; i++){
            char name[11];
            memcpy(name, directory->Filename, 11);
            name[11]=0;

            if(strncmp(DosFileName, name, 11) == 0){
                strcpy(file.name, DirectoryName);
                file.id = 0;
                file.currentCluster = directory->FirstCluster;
                file.eof = 0;
                file.filelength = directory->FileSize;

                if(directory->Attrib == 0x10){
                    file.flags = 2;
                } else {
                    file.flags = 1;
                }
                return file;
            }
            directory++;
        }
    }

    // Can't find file
    file.flags = -1;
    return file;

}


void fsysFATRead(PFILE file, unsigned char* buffer, unsigned int length){
    if(file){
        unsigned int physSector = 32 + (file->currentCluster - 1);
        const unsigned int  SECTOR_SIZE = 512;
        // read sector
        ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, physSector, (uint16_t*) buffer );

        unsigned int FAT_Offset = file->currentCluster + (file->currentCluster /2);
        unsigned int FAT_Sector = 1 + (FAT_Offset / SECTOR_SIZE);
        unsigned int entryOffset =FAT_Offset % SECTOR_SIZE;

        uint8_t FAT[SECTOR_SIZE*2];
        ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, FAT_Sector,(uint16_t*) FAT); // Read 1st FAT sector

        ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, FAT_Sector +1, (uint16_t*)FAT+SECTOR_SIZE);

        // read entry for next cluster
        uint16_t nextCluster = *(uint16_t*) &FAT[entryOffset];

        // test if entry is odd or even
        if(file->currentCluster & 0x0001){
            nextCluster>>= 4; // grab the high 12 bits
        }else{
            nextCluster &= 0x0FFF; // grab the low 12 bits
        }

        // test for end of file
        if(nextCluster >= 0xff8){
            file->eof -1;
            return;
        }

        // test for file corruption
        if(nextCluster == 0){
            file->eof =1;
            return;
        }

        // set next cluster
        file->currentCluster = nextCluster;


    }
}

FILE fsysFatOpenSubDir(FILE kFile, const char* filename){
    FILE file;

    char DosFileName[11];
    ToDosFileName(filename, DosFileName, 11);
    DosFileName[11] = 0;

    while(!kFile.eof){
        //read directory
        unsigned char buf[512];
        fsysFATRead(&file, buf, 512);

        PDIRECTORY  pkDir = (PDIRECTORY) buf;

        for (unsigned int i = 0; i < 16; i++){
            // get current filename
            char name[11];
            memcpy(name, pkDir->Filename, 11);
            name[11] = 0;

            if(strncmp(name, DosFileName, 11) == 0){
                strcpy(file.name, filename);
                file.id = 0;
                file.currentCluster = pkDir->FirstCluster;
                file.filelength = pkDir->FileSize;
                file.eof = 0;

                // set file type;
                if(pkDir->Attrib == 0x10){
                    file.flags = 2;
                } else{
                    file.flags = 1;
                }

                return file;
            }
            // go to next entry
            pkDir++;
        }
    }
    // unable to find file
    file.flags = -1;
    return file;
}
*/
