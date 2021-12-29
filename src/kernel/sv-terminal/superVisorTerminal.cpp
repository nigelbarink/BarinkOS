#include "superVisorTerminal.h"

void startSuperVisorTerminal(){
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
         

            if ( strncmp("DATE", command , characterCount ) == 0 )
            {
                 read_rtc();
                 printf("======= Time & Date ==========\n");
                 printf("   - Date:     %02d-%02d-%02d\n",day, month, year);
                 printf("   - Time:     %02d:%02d:%02d\n" , hour, minute, second);
                 printf("   - Ticks:    %09d\n", pit_tick);            
            }
            else if( strncmp ("MEMORY" , command , characterCount) == 0 )
            {
                // Show memory layout
                printf("========= Memory ==========\n");
                printf("Kernel MemoryMap:\n");
                printf("kernel: 0x%x - 0x%x\n", &kernel_begin , &kernel_end);         
                printf("Frames used: 0x%x blocks of 4 KiB\n", used_blocks); 
                //printf("\n\n");
                //PrintPhysicalMemoryAllocation(  );


            }
            else if(strncmp("TEST", command, characterCount) == 0)
            {
                // TEST #DE exception
               asm volatile ("MOV $4, %AX ;  MOV $0, %BX ; DIV %BX"); // IRS 0
            }
            else if (strncmp("VERSION", command , characterCount) == 0)
            {
                // Show version information 
                printf("========= Version ========\n");
                printf("Kernel v%d\n", 0);

            }
            else if(strncmp("CLEAR", command, characterCount) == 0)
            {
                kterm_init();
                printf("|===    BarinkOS       ===|\n");
            }
            else
            {
                printf("Unknown command\n");
            }


            delay(1000);
        }   
}