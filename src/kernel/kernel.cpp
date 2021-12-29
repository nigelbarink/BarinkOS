#include "kernel.h"
#define GB2 262144

 extern "C" void kernel_main (void);
  
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
         * Show a little banner for cuteness
         */
        printf("|===    BarinkOS       ===|\n");


        /**
         * Use the address given as an argument as the pointer
         * to a Multiboot information structure.
         */
          multiboot_info_t* mbt = (multiboot_info_t*) addr;
         
        /*
            If we got a memory map from our bootloader we 
            should be parsing it to find out the memory regions available.
         */
        if (CHECK_FLAG(mbt->flags, 6))
        {   
            printf("Preliminary results mmap scan:\n");
            mapMultibootMemoryMap(mbt);
            
            PhysicalMemoryManager_initialise( mbt->mmap_addr,  GB2/* Seriously dangerous hardcoded memory value*/);
            PhysicalMemoryManager_initialise_available_regions(mbt->mmap_addr, mbt->mmap_addr + mbt->mmap_length);
            PhysicalMemoryManager_deinitialise_kernel();
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

