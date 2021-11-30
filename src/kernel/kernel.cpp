#include "kernel.h"
#define GB4 524288
#define GB2 262144

int memcmp( const void* ptr1, const void* ptr2, size_t num);

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

    int memcmp( const void* ptr1, const void* ptr2, size_t num)
    {
        const unsigned char * cs = (const unsigned char*) ptr1;
        const unsigned char * ct = (const unsigned char*) ptr2;


        for (int i = 0 ; i < num ; i++, cs++, ct++ ){
           if( *cs < *ct){
               return -1;
           } else if( *cs > *ct){
               return 1;
           }
        } 

        return 0;

    }

    extern "C" void kernel_main (void) {

        printf("call to init serial\n");
        init_serial();
        print_serial("Serial port initialized!");

     

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

        // ATAPI_DEVICE::isPacketDevice();
 
    


        ATAPI_DEVICE::Identify(ATA_SECONDARY, DEVICE_DRIVE::MASTER);



        while (true){
            //Read time indefinetely 
            read_rtc();
            printf( "UTC time: %02d-%02d-%02d %02d:%02d:%02d  [ Formatted as YY-MM-DD h:mm:ss]\r" ,year, month, day, hour, minute, second);
            delay(1000);
        }
            

        
       
    }   

