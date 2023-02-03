extern "C"{
    #include "../lib/include/string.h" 
}
#include "definitions.h"

#include "prekernel/bootstructure.h"

#include "drivers/vga/VBE.h"
#include "drivers/pit/pit.h"

#include "memory/memory.h"
#include "memory/VirtualMemoryManager.h"
#include "memory/KernelHeap.h"
#include "memory/gdt/gdtc.h"

#include "drivers/acpi/rsdp.h"
#include "drivers/ide/ide.h"
#include "drivers/ata/ataDevice.h"
#include "PartitionTable/MBR/MasterBootRecord.h"
#include "interrupts/idt/idt.h"
#include "filesystem/FAT/BiosParameterBlock.h"
#include "filesystem/FAT/DirectoryEntry.h"
#include "drivers/io/io.h"
#include "drivers/pci/pci.h"
#include "cpu.h"
#include "serial.h"
#include "time.h"

#include "terminal/kterm.h"

#include "supervisorterminal/superVisorTerminal.h"

extern "C" void kernel_main ();
void ProcessBootInfo();

extern "C" void kernel_main ()
{
    /*
     * Show a little banner for cuteness
     */
    printf("|===    BarinkOS       ===|\n");
    startSuperVisorTerminal();

        RSDPTR* rsd = FindRSD();
        RSDT* rsd_table = getRSDT(rsd);


        // Enumerate the PCI bus
        PCI_Enumerate();  
        TestIDEController();      

        int devNumber = 0 ;
        for ( auto device : ide_devices){
            if (!device.Reserved)
                continue;
            printf("Device %d\n" , devNumber);
            printf (" Device on Channel: (0x%x) %s\n" ,device.Channel, device.Channel == 0 ? "Primary" : "Secondary");
            printf (" Device drive:(0x%x) %s\n" , device.Drive,  device.Drive? "Slave" : "Master");
            printf (" Device Type:(0x%x) %s\n" , device.Type, device.Type ? "ATAPI" : "ATA");
            devNumber ++;

        }

        enum BUS_PORT { 
            Primary= 0x1f0,
            Secondary = 0x170
        };


        
        ATA_DEVICE::Identify((uint16_t) BUS_PORT::Primary, DEVICE_DRIVE::MASTER);

        const int C = 0; 
        const int H = 0; 
        const int HPC = 16;
        const int SPT = 63;

        int S = 1; 
        uint32_t LBA = (C*HPC+H) * SPT + (S-1);
        printf("LBA: %d\n" , LBA);
        uint16_t buffer [256];


        ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, LBA, buffer);
   
        MBR* mbr = (MBR*) buffer;

        printf("BootSector: 0x%x\n", mbr->ValidBootsector );
        for( int i = 0 ; i < 4 ; i ++){
             PartitionTableEntry PT = mbr->TableEntries[i];
   
             printf("Partition %d [  %d sectors,  PartitionType: %x, 0x%x, \nLBA Start: 0x%x ]\n" ,
              i, PT.Number_sectors_inPartition, PT.PartitionType, mbr->uniqueID,  PT.LBA_partition_start );
         }

         // Find the BiosParameter block
        uint16_t biosparameterblock[256];
        ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, mbr->TableEntries[0].LBA_partition_start, biosparameterblock);

        BiosParameterBlock*  bpb =  (BiosParameterBlock*) biosparameterblock;
        

        printf("\nBPB: Bytes per Sector %d\n", bpb->BytesPerSector );
        printf("OEM ID: %s\n", bpb->OEM_id);
        printf("Bytes per sector: %d\n", bpb->BytesPerSector);
        printf("Sectors per cluster: %d\n", bpb->SectorsPerCluster);
        printf("Reserved sectors: %d\n", bpb->ReservedSectors);
        printf("Number of FAT: %d\n", bpb->NumberOfFileAllocationTables);
        printf("Number of Dir entries: %d\n", bpb->NumberOfDirectoryEntries);
        printf("Total Sectors in volume: %d\n", bpb->TotalSectorsInLogicalVolume);
        printf("Sectors per FAT: %d\n", bpb->NumberOfSectorsPerFAT);

        
        

        /**
         * @brief File Allocation Table 
         */


        uint32_t FATAddress = mbr->TableEntries[0].LBA_partition_start +  bpb->ReservedSectors ;
        uint16_t FAT[256];
        ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, FATAddress, FAT );

        // Show data in terminal
        for(int i = 0; i < 256; i++ ) {
            printf("%x ", FAT[i]);
        }
        kterm_put('\n');


         uint32_t RootDirectoryRegion = FATAddress + ( bpb->NumberOfFileAllocationTables * bpb->NumberOfSectorsPerFAT );
         uint32_t DataRegion = RootDirectoryRegion + ((bpb->NumberOfDirectoryEntries * 32) / bpb->BytesPerSector );

         uint16_t data2 [256];
         ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, RootDirectoryRegion, data2 );
         DirectoryEntry* RootDirectory = (DirectoryEntry*) data2;
         // List files in root
          for(int i= 0; i < bpb->NumberOfDirectoryEntries ; i++ )
          {  
            DirectoryEntry* entry = (DirectoryEntry*)((uint32_t) RootDirectory + (i  * sizeof(DirectoryEntry)));

            if( entry->filename[0] == (uint8_t) 0x00 )
                break; // There are no more entries in this directory or the entry is free
             
            if( entry->attribute & 0x01 == 0x01 || entry->attribute & 0x20 == 0x20)
                continue; // Skip listing if hidden or Achieve flag is set

            // Print the filename;
            for( int n = 0; n < 8; n++ ){
                if(entry->filename[n] == 0x20)
                    break;
                kterm_put(entry->filename[n]);
            }kterm_put('\n');

            for( int n = 0; n < 3; n++){
                kterm_put(entry->Extension[n]);
            }kterm_put('\n');

            printf("Attribute: %x \n" , entry->attribute);
            printf("FileSize: %d Bytes\n", entry->FilesizeInBytes);
    
            if( entry->FilesizeInBytes != 0x0 || entry->attribute & 0x8 == 0x0){
                printf("Show contents");

                printf( "Start cluster of the file: 0x%x\n" , entry->StartingCluster);

                printf("IS it only 1 cluster? %s\n" , FAT[i] == 0xFFFF? "Yes": "No" );

                uint32_t sector = DataRegion + ((entry->StartingCluster - 0x02 ) * bpb->SectorsPerCluster);


                uint16_t dataBlob [256];
                ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, sector, dataBlob );
                for( int n = 0; n < 256; n++)
                {
                    kterm_put(dataBlob[n] & 0x00ff);

                    kterm_put(dataBlob[n] >> 8);
                }kterm_put('\n');


            }
    
            printf("======================\n");
            
    
          }
}   


extern "C" void early_main()
{
    init_serial();

    kterm_init();
    printf("Allocated blocks: 0x%x \n", GetUsedBlocks());

    initGDT();
    init_idt();

    // Enable interrupts
    asm volatile("STI");
   

    initHeap();

    printf("TRY ALLOCATING 4 BYTES\n");
    uint32_t* MyVariable = (uint32_t*) malloc(4); // allocate 4 bytes using my heap
    free(MyVariable);

    // test heap allocation
    
    struct KernelInfo {
        int bar;
        bool foo;
    };

    KernelInfo* MyInfo = (KernelInfo*) malloc(sizeof(KernelInfo));

    MyInfo->bar = 6;
    MyInfo->foo = false;
    printf("bar contains %d\n", MyInfo->bar);
    free(MyInfo);
    


    printf("Enable Protected mode and jump to kernel main\n");

	asm volatile("mov %cr0, %eax ");
    asm volatile("or $1, %eax");
    asm volatile("mov %eax, %cr0"); // re-enable protected mode ? 

    pit_initialise();


    kernel_main();
    
}
