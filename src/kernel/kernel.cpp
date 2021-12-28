#include "kernel.h"
#define GB2 262144

 extern "C" void kernel_main (void);
  
    extern "C" void early_main(unsigned long magic, unsigned long addr){
        /** initialize terminal interface */ 
        kterm_init();
        
        // Check Multiboot magic number 
        if (magic != MULTIBOOT_BOOTLOADER_MAGIC){
            printf("Invalid magic number: 0x%x\n",  magic);
            return;
        }

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
            printf("Frames used: 0x%x blocks of 4 KiB\n", used_blocks); 
        }
        
        initGDT();
        init_idt();
        // Enable interrupts
        asm volatile("STI");
        

        CheckMBT(  (multiboot_info_t *) addr);


        kernel_main();
       
    }

    extern "C" void kernel_main (void) {
        init_serial();
        pit_initialise();

        while (true){
           
           printf("SUPERVISOR:>$ " );
            int characterCount = 0;
            char command[10] = "";
         
            // NOTE: lets just show a kernel prompt                
            uint8_t ScanCode = getKey();
            while( ScanCode  != 0x1C )
            {
                char character = getASCIIKey();
                kterm_put(character );
                // wHAT THE HELL

                if( characterCount  < 10 ){
                    command[characterCount] = character;  
                    characterCount++;
                }

                ScanCode = getKey(); 
            }
            printf("\n");
            KeyHandled();
         

            if ( strncmp("TIME", command , characterCount ) == 0 ) {
                 read_rtc();
                 printf( "UTC time: %02d-%02d-%02d %02d:%02d:%02d (Ticks: %06d)\n" ,year, month, day, hour, minute, second, pit_tick);            
            } else if(strncmp("TEST", command, characterCount) == 0){
               // asm volatile ("MOV $4, %AX ;  MOV $0, %BX ; DIV %BX"); // IRS 0
            }
            else{
                printf("Unknown command\n");
            }


            delay(1000);
        }   


       

    }   

