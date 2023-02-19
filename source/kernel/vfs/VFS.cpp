#include "VFS.h"
#include "../filesystem/FAT/BiosParameterBlock.h"
#include "../drivers/ide/ide.h"
#include "../drivers/ata/ataDevice.h"
#include "../partitiontable/mbr/MasterBootRecord.h"
#include "../memory/KernelHeap.h"
#include "Path.h"
#include "../filesystem/FAT/DirectoryEntry.h"

MOUNT_INFO mountInfo;

PFILESYSTEM _filesystems[DEVICE_MAX];

FILE volOpenFile(const char* fname)
{
    if(fname){
        unsigned char device = 'a';

        char* filename = (char*) fname;

        if(fname[1]== ':'){
            device = fname[0];
            filename += 2; // strip the volume component from the path
        }

        if(_filesystems[device - 'a']){
            FILE file = _filesystems[device-'a']->Open(filename);
            file.device = device;
            return file;
        }
    }

    FILE file;
    file.flags = FS_INVALID;
    return file;
}

extern void volReadFile(PFILE file, unsigned char* Buffer, unsigned int length)
{

}
extern void volCloseFile(PFILE file)
{
    if( file->device < DEVICE_MAX){
    //    _filesystems[file->device]->Close(file);
    }
}

extern void volRegisterFilesystem(PFILESYSTEM fsys , unsigned int deviceID){
    if(deviceID < DEVICE_MAX)
        if(fsys)
            _filesystems[deviceID] = fsys;
}

extern void volUnregisterFilesystem(PFILESYSTEM){

}

extern void volUnregisterFileSystemByID(unsigned int deviceID){

}

enum BUS_PORT {
    Primary = 0x1f0,
    Secondary = 0x170
};

bool driveAvailable(){
    int devNumber = 0;
    for ( auto device : ide_devices){
        if(!device.Reserved)
            continue;
        devNumber++;
    }


    // FIXME: If no drive is connected we continue trying to read from
    // a not connected drive!
    ATA_DEVICE::Identify((uint16_t) BUS_PORT::Primary, DEVICE_DRIVE::MASTER);
    return true;
}

MBR* getPartitions(bool DEBUG = false){
    const int C = 0;
    const int H = 0;
    const int HPC = 16;
    const int SPT = 63;

    int S =1;
    uint32_t LBA = (C*HPC+H) * SPT + (S-1);
    MBR* mbr =(MBR*) malloc(sizeof (MBR));

    ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, LBA, (uint16_t*)mbr);

    if(DEBUG){
        printf("BootSector: 0x%x\n", mbr->ValidBootsector );
        for( int i = 0 ; i < 4 ; i ++){
            PartitionTableEntry PT = mbr->TableEntries[i];

            printf("Partition %d [  %d sectors,  PartitionType: 0x%x, 0x%x, \nLBA Start: 0x%x ]\n" ,
                   i, PT.Number_sectors_inPartition, PT.PartitionType, mbr->uniqueID,  PT.LBA_partition_start );
        }

    }

    return mbr;
}

BiosParameterBlock* getBPB(MBR* mbr, bool DEBUG =false ){
    BiosParameterBlock* bpb = (BiosParameterBlock*) malloc(sizeof(BiosParameterBlock));
    ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, mbr->TableEntries[0].LBA_partition_start, (uint16_t*) bpb);

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

uint16_t* ReadFAT (BiosParameterBlock& bpb , MBR& mbr, bool DEBUG = false ) {
    uint32_t FATAddress = mbr.TableEntries[0].LBA_partition_start +  bpb.ReservedSectors ;
    uint16_t* FAT = (uint16_t*)malloc(sizeof (uint16_t) * 256);
    ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, FATAddress, FAT );

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
    ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, sector, dataBlob);
    for (unsigned short n: dataBlob) {
        kterm_put(n & 0x00ff);

        kterm_put(n >> 8);
    }
    kterm_put('\n');
}
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

        if ((entry->attribute & 0x01) == 0x01 || (entry->attribute & 0x20) == 0x20)
            continue; // Skip listing if hidden or Achieve flag is set

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

        if (entry->FilesizeInBytes != 0x0  && (entry->attribute == 0x08)) {
            readFile(DataRegion,entry, FAT[i], bpb);
        }

    }
    free(FAT);

}


/*
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
 */

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
/*
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

void FileSystem::initialize()
{
    MBR* mbr = getPartitions();
    BiosParameterBlock* bootsector =  getBPB(mbr);
    listFilesInRoot(*mbr, *bootsector);

    free(mbr);
    free(bootsector);
/*
    mountInfo.numSectors =  bootsector->NumberOfSectorsPerFAT;
    mountInfo.fatOffset = 1;
    mountInfo.fatSize = bootsector->NumberOfSectorsPerFAT;
    mountInfo.fatEntrySize = 8;
    mountInfo.numRootEntries = bootsector->NumberOfDirectoryEntries;
    mountInfo.rootOffset = (bootsector->NumberOfFileAllocationTables * bootsector->NumberOfSectorsPerFAT) + 1;
    mountInfo.rootSize = (bootsector->NumberOfDirectoryEntries * 32) / bootsector->BytesPerSector;
*/


}

char* FindNextEntryName (char* path )
{
    int length = strlen(path);

    char* name = path;
    int i = 0;

    if( name[0] == '/')
        i++;

    while ( name[i] != '/' && i <= length)
        i++;

    char* s = (char*) malloc(i + 1 * sizeof(char));
    for ( int a = 0; a < i; a++)
        s[a] = path[a];

    s[i + 1] = '\0';

    return s;

}



void FileSystem::ResolvePath(Path &path)
{
    char* string_path  = path.str();
    void* cpy = string_path;

    bool isAbsolutePath = string_path[0] == '/';
    if(isAbsolutePath)
    {
        // strip the first slash
        string_path++;
    }

    char* entry_name = FindNextEntryName(string_path);
    printf("Look for entry with name: %s\n", entry_name);
    int skip = strlen(entry_name);
    free(entry_name);


    entry_name = FindNextEntryName(string_path + skip);
    printf("Look for entry with name: %s\n", entry_name);
    skip = strlen(entry_name);
    free(entry_name);




    free(cpy);

}