#include "kernel.h"
#define GB4 524288
#define GB2 262144
extern "C" void kernel_main (void);
extern "C" void putPixel(int pos_x, int pos_y, unsigned char VGA_COLOR , unsigned char addr , unsigned char pixelWidth, unsigned pitch );


    extern "C" void early_main(unsigned long magic, unsigned long addr){
        /** initialize terminal interface */ 
       // kterm_init();
        
        printf("Magic flag 0x%8x\n", magic);
        printf("Magic must be 0x%8x\n", MULTIBOOT_BOOTLOADER_MAGIC);
        if (magic != MULTIBOOT_BOOTLOADER_MAGIC){
            printf("Invalid magic number: 0x%8x\n",  magic);
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


        while (true){
            //Read time indefinetely 
            read_rtc();
            printf( "UTC time: %02d-%02d-%02d %02d:%02d:%02d  [ Formatted as YY-MM-DD h:mm:ss]\r" ,year, month, day, hour, minute, second);
            delay(1000);
        }
            

        
       
    }   

