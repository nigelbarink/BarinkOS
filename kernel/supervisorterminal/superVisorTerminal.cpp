#include "superVisorTerminal.h"
#include "../storage/ata pio/ATAPIO.h"
#include "../storage/partitiontables/mbr/MasterBootRecord.h"
#include "../storage/filesystems/FAT/FAT.h"

bool isRunning = true;
extern "C" void startSuperVisorTerminal()
{


    /*
    * Show a little banner for cuteness
    */
    printf("|===    BarinkOS       ===|\n");

    while (isRunning){
           
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
            if( strncmp ("MEMORY" , command , characterCount) == 0 )
            {
                // Show memory layout
                printf("========= Memory (very inaccurate) ==========\n");
                printf("Kernel MemoryMap:\n");
                printf("kernel: 0x%x - 0x%x\n", &kernel_begin , &kernel_end);         
                printf("Frames used: %d blocks of 4 KiB\n", GetUsedBlocks());  

            }
            if(strncmp("TEST", command, characterCount) == 0)
            {
                // TEST #DE exception
               asm volatile ("MOV $4, %AX ;  MOV $0, %BX ; DIV %BX"); // IRS 0
            }
            if (strncmp("VERSION", command , characterCount) == 0)
            {
                // Show version information 
                printf("========= Version ========\n");

                asm volatile ("movl $0x666, %eax; int $0x50");

            }
            if(strncmp("CLEAR", command, characterCount) == 0)
            {
                kterm_init();
                printf("|===    BarinkOS       ===|\n");
            }
            if(strncmp("LIST", command, 4) == 0)
            {

                // slice off the command part
                char args[characterCount - 4];
                for(int i = 5 ; i < characterCount; i++) {
                    args[i] = command[i];
                }

                printf("=============== DIRECTORY LISTING =================\n");
                printf("Path to show %s\n", args);
            }

            if(strncmp("DEVICES", command, characterCount) == 0){
                printf("================ CONNECTED DEVICES ===============\n");

            }


            printf("executed command: %s\n", command);


            delay(1000);
        }   
}