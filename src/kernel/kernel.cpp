#include "kernel.h"
#include "memory/PageFrameAllocator.h"

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
            mapMultibootMemoryMap(mbt);

        }

        /* Draw diagonal blue line */
        if (CHECK_FLAG (mbt->flags, 12)){
            printf("Can draw!");
        } 

        //setupGdt();

       
    }

    extern "C" void kernel_main (void) {

        init_serial();

        while (false){
            //Read time indefinetely 
            read_rtc();
            printf( "UTC time: %02d-%02d-%02d %02d:%02d:%02d  [ Formatted as YY-MM-DD h:mm:ss]\r" ,year, month, day, hour, minute, second);
            delay(1000);
        }
       
    }   

