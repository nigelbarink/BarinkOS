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
        
        int devicesFound = 0;
        // loop through all possible busses, devices and their functions;
        for( int bus = 0 ; bus < 256 ; bus++)
        {
            
            for(int device = 0; device < 32 ; device ++)
            {
                for ( int function = 0; function < 8; function++)
                {
                
                    uint64_t DeviceIdentify = ConfigReadWord(bus, device, function,0x0);
                    uint32_t VendorID  = DeviceIdentify & 0xFFFF;
                    uint32_t DeviceID = DeviceIdentify >> 16;



                    if( DeviceID != 0xFFFF){
                        printf("bus: %d, device: %d, function %d \n");
                        printf("Device found!\n");
                        printf("DeviceID: 0x%x, VendorID: 0x%x\n", DeviceID,  VendorID);

                        uint32_t classcodes = ConfigReadWord(bus, device, function, 0x8);
                        uint32_t classData = classcodes >> 16; // We only care for the last 2 bytes!
                        uint32_t deviceClass = classData >> 8;
                        uint32_t subclass = classData & 0xFF;

                        printf(" class: %d, subClass: %d\n\n", deviceClass, subclass);
                        devicesFound++;
                
                    }
                
                
                
                }

            }
            
        }

        printf("Found %d devices!", devicesFound);

        while (true){
            //Read time indefinetely 
            //read_rtc();
            //printf( "UTC time: %02d-%02d-%02d %02d:%02d:%02d  [ Formatted as YY-MM-DD h:mm:ss]\r" ,year, month, day, hour, minute, second);
            delay(1000);
        }
            

        
       
    }   

