#include "kernel.h"
#define GB4 524288
#define GB2 262144


extern "C" void kernel_main (void);

    extern "C" void early_main(unsigned long magic, unsigned long addr){
       /** initialize terminal interface */ 
        kterm_init();

        if (magic != MULTIBOOT_BOOTLOADER_MAGIC){
            printf("Invalid magic number: 0x%x\n",  magic);
            return;
        }
        
        CheckMBT(  (multiboot_info_t *) addr);

        multiboot_info_t* mbt = (multiboot_info_t*) addr;

        /* Are mmap_* valid? */
        if (CHECK_FLAG(mbt->flags, 6)){
            PhysicalMemoryManager_initialise( mbt->mmap_addr,  GB2/* Seriously dangerous hardcoded memory value*/);
            PhysicalMemoryManager_initialise_available_regions(mbt->mmap_addr, mbt->mmap_addr + mbt->mmap_length);
            PhysicalMemoryManager_deinitialise_kernel();
            extern uint8_t* kernel_begin;
            extern uint8_t* kernel_end;

            printf("Kernel MemoryMap:\n");
            printf("kernel: 0x%x - 0x%x\n", &kernel_begin , &kernel_end);         
        }

        initGDT();
     

        kernel_main();
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

         // Find the super block
        uint16_t superBlock[256];
        ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, mbr->TableEntries[0].LBA_partition_start, superBlock);

        BiosParameterBlock*  bpb =  (BiosParameterBlock*) superBlock;
        

        printf("\nBPB: Bytes per Sector %d\n", bpb->BytesPerSector );
        printf("OEM ID: %s\n", bpb->OEM_id);
        printf("Bytes per sector: %d\n", bpb->BytesPerSector);
        printf("Sectors per cluster: %d\n", bpb->SectorsPerCluster);
        printf("Reserved sectors: %d\n", bpb->ReservedSectors);
        printf("Number of FAT: %d\n", bpb->NumberOfFileAllocationTables);
        printf("Number of Dir entries: %d\n", bpb->NumberOfDirectoryEntries);
        printf("Total Sectors in volume: %d\n", bpb->TotalSectorsInLogicalVolume);
        printf("Sectors per FAT: %d\n", bpb->NumberOfSectorsPerFAT);

        uint32_t PartitionAddress = mbr->TableEntries[0].LBA_partition_start *512 ;
        uint32_t RootDirAddress = PartitionAddress + ((bpb->ReservedSectors + bpb->NumberOfSectorsPerFAT * bpb->NumberOfFileAllocationTables ) * bpb->BytesPerSector);
        uint32_t RootDirLBA =RootDirAddress/512;
        
        
     
        uint16_t RootDir [16];
        ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER,RootDirLBA, (uint16_t*) RootDir );
     



        while (true){
            //Read time indefinetely 
            read_rtc();
            printf( "UTC time: %02d-%02d-%02d %02d:%02d:%02d  [ Formatted as YY-MM-DD h:mm:ss]\r" ,year, month, day, hour, minute, second);
            delay(1000);
        }
        
    }   
