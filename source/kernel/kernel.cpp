#include "kernel.h"
#define GB4 524288
#define GB2 262144



    extern "C" void wait_until_shutdown(){
        while (true){
            //Read time indefinetely 
            read_rtc();
            printf( "UTC time: %02d-%02d-%02d %02d:%02d:%02d  [ Formatted as YY-MM-DD h:mm:ss]\r" ,year, month, day, hour, minute, second);
            delay(1000);
        }

    }


    extern "C" void kernel_main (void) {

        printf("call to init serial\n");
        init_serial();
        print_serial("Serial port initialized!");

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

        wait_until_shutdown();
        
    }   




 

extern "C" void early_main(unsigned long magic, unsigned long addr){
    /** 
     * Initialize terminal interface 
     * NOTE: This should be done later on , the magic value should be checked first.
     */ 
    kterm_init();
    
    /**
     * Check Multiboot magic number
     * NOTE: Printf call should not be a thing this early on ...
    */ 
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC){
        printf("Invalid magic number: 0x%x\n",  magic);
        return;
    }

    /**
     * Use the address given as an argument as the pointer
     * to a Multiboot information structure.
     */
        multiboot_info_t* mbt = (multiboot_info_t*) addr;

    /**
     * Construct our own bootInfo structure
     */
        BootInfo bootinfo = {};
       
        
    /*
        If we got a memory map from our bootloader we 
        should be parsing it to find out the memory regions available.
        */
    if (CHECK_FLAG(mbt->flags, 6))
    {  

        /*
            Setup Physical memory managment
        */
        MemoryInfo meminfo = {};
        bootinfo.memory = &meminfo;
 
        mapMultibootMemoryMap(bootinfo.memory , mbt);        
        printf("Memory size: 0x%x bytes\n", bootinfo.memory->TotalMemory );

        PhysicalMemory memAlloc = PhysicalMemory{};
        memAlloc.setup(bootinfo.memory );

        /*
         Mark already in use sections 
        */
        
        // Mark kernel memory as used
        printf("Kernel Begin Pointer: 0x%x, Kernel end pointer: 0x%x\n", kernel_begin , kernel_end );
       
        multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) mbt->mmap_addr;

        for (;  (unsigned long) mmap < mbt->mmap_addr + mbt->mmap_length;  mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size))){

            if ( mmap->type == MULTIBOOT_MEMORY_AVAILABLE){
                  
            } else{
                printf("allocate region: 0x%x, size : 0x%x bytes\n", (unsigned) mmap->addr,(unsigned) mmap->len  );
                memAlloc.allocate_region((unsigned)mmap->addr , (unsigned)mmap->len);
            }
          
            
        } 


       printf("allocate region: 0x%x, size : 0x%x bytes\n", kernel_begin, kernel_end - kernel_begin );
       memAlloc.allocate_region(kernel_end, kernel_end - kernel_begin);

        // test alloc_block
     
         
        uint8_t* memory = (uint8_t*) memAlloc.allocate_block();
        printf("Got a new pointer: 0x%x\n", memory);

        uint8_t* memory2 = (uint8_t*) memAlloc.allocate_block();
        printf("Got a new pointer: 0x%x\n", memory2);


        memAlloc.free_block((void*) memory);

        uint8_t* newBlockPlse = (uint8_t*) memAlloc.allocate_block();
        



       // memAlloc.free_block((void*) memory);
  
    
    }
    
    initGDT();
    init_idt();
    // Enable interrupts
    asm volatile("STI");
         

    init_serial();
    pit_initialise();


    CheckMBT(  (multiboot_info_t *) addr);

    startSuperVisorTerminal(&bootinfo);

    kernel_main();
    
}


