/*
    Copyright © Nigel Barink 2023
*/
#include "memory/memory.h"
#include "memory/KernelHeap.h"
#include "memory/gdt/gdtc.h"
#include "memory/TaskStateSegment.h"
#include "supervisorterminal/superVisorTerminal.h"
#include "drivers/vga/VBE.h"
#include "pci/pci.h"
#include "drivers/pit/pit.h"
#include "acpi/acpi.h"
#include "i386/processor.h"
#include "terminal/kterm.h"
#include "interrupts/idt.h"
#include "serial.h"
#include "storage/vfs/vfs.h"
#include "storage/filesystems/FAT/FAT.h"


extern "C"  void LoadGlobalDescriptorTable();
extern "C" void jump_usermode();
extern BootInfoBlock* BIB;

extern "C" void kernel ()
{

    init_serial();
    kterm_init();

    setup_tss();
    initGDT();
    initidt();
    LoadGlobalDescriptorTable();
    flush_tss();
    printf("Memory setup complete!\n");

    // Enable interrupts
    asm volatile("STI");

    initHeap();
    pit_initialise();
    ACPI::initialize();

    PCI::Scan();
    processor::initialize();
    processor::enable_protectedMode();
    printf("Boot device: 0x%x\n", BIB->bootDeviceID);
    unsigned int part3 = BIB->bootDeviceID & 0xFF;
    unsigned int part2 = (BIB->bootDeviceID & 0xFF00) >> 8;
    unsigned int part1 = (BIB->bootDeviceID & 0xFF0000) >> 16;
    unsigned int drive = (BIB->bootDeviceID & 0xFF000000) >> 24;
    if (drive == 0x80 )
        printf("booted from disk!\n");
    if(drive == 0x00)
        printf("booted from floppy disk\n");

    printf("Part1: %d, Part2: %d, Part3: %d\n", part1, part2 , part3);
    ATAPIO::Identify(ATAPIO_PORT::Primary, DEVICE_DRIVE::MASTER);
    auto* bpb = FAT::getBPB(false);
    auto* mbr = GetPartitions(false);
    auto fsType = FAT::determineFATType(bpb);
    switch (fsType) {
        case FAT_TYPE::FAT12:
            printf("FAT12 Disk!\n");
            break;
        case FAT_TYPE::FAT16:
            printf("FAT16 Disk!\n");
            break;
        case FAT_TYPE::FAT32:
            printf("FAT32 Disk!\n");
            break;
    }


    // list files in root
    int total_sectors = bpb->TotSec32;
    int fat_size = bpb->FATSz16;
    int root_dir_sectors = FAT::RootDirSize(bpb);
    int first_data_sector = bpb->RsvdSecCnt + (bpb->NumFATs * fat_size) + root_dir_sectors ;
    int first_fat_sector = bpb->RsvdSecCnt;
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
            printf("Long file name detected!\n");
        }
        printf(" [Size: %d bytes, Attributes: %x\n", entry->ATTR, entry->FileSize);


    }






 //   VirtualFileSystem::initialize();

   // VirtualFileSystem::open("/hello.txt", 0);


#ifdef USERMODE_RELEASE
    // Lets jump into user mode
    jump_usermode();
#else
    startSuperVisorTerminal();
#endif

}