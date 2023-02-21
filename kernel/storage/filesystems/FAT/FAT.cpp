//
// Created by nigel on 21/02/23.
//
#include "FAT.h"
#include "../../ata pio/ATAPIO.h"
#include "../../../memory/KernelHeap.h"
#include <CoreLib/Memory.h>

void FAT::Read(PFILE file, unsigned  char* buffer , unsigned int length)
{

}

FILE FAT::Open(char* filename)
{

    char* tokstate = NULL;
    char* nextdir = strtok(filename, "/", &tokstate);

    while (nextdir)
    {

        // Read the root directory
        printf("First entry to look for: %s\n", nextdir);



        nextdir = strtok(NULL, "/", &tokstate);
    }

    FILE file;
    file.flags = FS_INVALID;
    return file;

}

void FAT::Write(PFILE file, unsigned char* buffer, unsigned int length)
{

}

void ParseDateInteger(unsigned int date){
    printf("Date (hex) 0x%x\n", date);
    unsigned int year = (date >> 9 )+ 1980;
    unsigned int month = (date & 0xf0   ) >> 4;
    unsigned int day = date & 0xf ;
    printf("Date: (D,M,Y) %d, %d ,%d\n", day , month, year );

}


BiosParameterBlock* getBPB(PTR_PARTITION partition, bool DEBUG =false ){
    BiosParameterBlock* bpb = (BiosParameterBlock*) malloc(sizeof(BiosParameterBlock));

    ATAPIO_PORT port = (ATAPIO_PORT)(partition->Disk & 0x01FF);
    DEVICE_DRIVE drive = (DEVICE_DRIVE)(partition->Disk >> 16);

    printf("ATAPIO_PORT: 0x%x DEVICE_DRIVE: 0x%x\n",port, drive);
    printf("Partition Start Address (LBA): 0x%x\n", partition->StartAddress);

    ATAPIO::Read(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER, partition->StartAddress, (uint16_t*) bpb);

    if(DEBUG)
    {
        printf("OEM ID: %s\n", bpb->OEM_id);
        printf("Bytes per sector: %d\n", bpb->BytesPerSector);
        printf("Sectors per cluster: %d\n", bpb->SectorsPerCluster);
        printf("Reserved sectors: %d\n", bpb->ReservedSectors);
        printf("Number of FAT: %d\n", bpb->NumberOfFileAllocationTables);
        printf("Number of Dir entries: %d\n", bpb->NumberOfDirectoryEntries);
        printf("Total Sectors in volume: %d\n", bpb->TotalSectorsInLogicalVolume);
        printf("Sectors per FAT: %d\n", bpb->NumberOfSectorsPerFAT);
    }

    return bpb;
}

bool FAT::Validate(PTR_PARTITION partition )
{

    auto* bootParams = getBPB(partition, true);

    if(bootParams->OEM_id) {
        return true;
    }

    return false;

}

void FAT::Info(_PARTITION *pPartition, PFS pSystem) {
    pSystem->Read = FAT::Read;
    pSystem->Write = FAT::Write;
    pSystem->Open = FAT::Open;


}

uint16_t* ReadFAT (BiosParameterBlock& bpb , PTR_PARTITION partition, bool DEBUG = false ) {
    uint32_t FATAddress = partition->StartAddress +  bpb.ReservedSectors ;
    uint16_t* FAT = (uint16_t*)malloc(sizeof (uint16_t) * 256);

    ATAPIO_PORT port = (ATAPIO_PORT)(partition->Disk & 0x01FF);
    DEVICE_DRIVE drive = (DEVICE_DRIVE)(partition->Disk >> 16);
    ATAPIO::Read(port, drive, FATAddress, FAT );

    // Show data in terminal
    if(DEBUG){
        for( unsigned int i =0 ; i < 256 ; i++) {
            printf("0x%x ", (unsigned short)FAT[i]);
        }
        kterm_put('\n');
    }

    return FAT;
}

void readFile(uint32_t DataRegion, DirectoryEntry* entry, uint16_t FATentry, BiosParameterBlock& bpb ){
    printf("Show contents");

    printf("Start cluster of the file: 0x%x\n", entry->StartingCluster);

    printf("IS it only 1 cluster? %s\n", FATentry == 0xFFFF ? "Yes" : "No");

    uint32_t sector = DataRegion + ((entry->StartingCluster - 0x02) * bpb.SectorsPerCluster);


    uint16_t dataBlob[256];
    ATAPIO::Read(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER, sector, dataBlob);
    for (unsigned short n: dataBlob) {
        kterm_put(n & 0x00ff);

        kterm_put(n >> 8);
    }
    kterm_put('\n');
}
/*
void listFilesInRoot(MBR& mbr, BiosParameterBlock& bpb ){
    auto FATAddress = mbr.TableEntries[0].LBA_partition_start +  bpb.ReservedSectors;
    uint32_t RootDirectoryRegion = FATAddress + ( bpb.NumberOfFileAllocationTables * bpb.NumberOfSectorsPerFAT );
    uint32_t DataRegion = RootDirectoryRegion + ((bpb.NumberOfDirectoryEntries * 32) / bpb.BytesPerSector );
    uint16_t* FAT = ReadFAT(bpb, mbr);

    uint16_t data2 [256];
    ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, RootDirectoryRegion, data2 );
    auto* RootDirectory = (DirectoryEntry*) data2;

    // List files in root
    for(int i= 0; i < sizeof (data2) / sizeof (DirectoryEntry); i++ ) {
        auto *entry = (DirectoryEntry * )((uint32_t) RootDirectory + (i * sizeof(DirectoryEntry)));

        if (entry->filename[0] == (uint8_t) 0x00)
            continue; // There are no more entries in this directory or the entry is free

        if (entry->attribute & ATTRIBUTES::ATT_HIDDEN)
            continue;
        if(entry->attribute & ATTRIBUTES::ATTR_SYSTEM)
            continue;
        if(entry->attribute & ATTRIBUTES::ATTR_VOLUME_ID)
            continue;

        // Print the filename;
        for (char n: entry->filename) {
            if (n == 0x20)
                break;
            kterm_put(n);
        }
        for (unsigned char n: entry->Extension) {
            kterm_put(n);
        }
        kterm_put('\n');


        printf("Attribute: %x \n", entry->attribute);
        printf("FileSize: %d Bytes\n", entry->FilesizeInBytes);

        if (entry->FilesizeInBytes != 0x0  && (entry->attribute != 0x10)) {
            readFile(DataRegion,entry, FAT[i], bpb);
        }

    }
    free(FAT);

}

FILE fsysFatDirectory (const char* DirectoryName){
    FILE file;
    unsigned char* buf;
    PDIRECTORY directory;

    char DosFileName[11];
    ToDosFileName(DirectoryName, DosFileName, 11);
    DosFileName[11] =0;

    for (int sector=0; sector <14 ; sector++){
        ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, mountInfo.rootOffset + sector, (uint16_t*)buf);
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
                    file.flags = FS_DIRECTORY;
                } else {
                    file.flags = FS_FILE;
                }
                return file;
            }
            directory++;
        }
    }

    // Can't find file
    file.flags = FS_INVALID;
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
                    file.flags = FS_DIRECTORY;
                } else{
                    file.flags = FS_FILE;
                }

                return file;
            }
            // go to next entry
            pkDir++;
        }
    }
    // unable to find file
    file.flags = FS_INVALID;
    return file;
}
 */
