#include "superVisorTerminal.h"
#include "../drivers/ata/ataDevice.h"
#include "../partitiontable/mbr/MasterBootRecord.h"
#include "../filesystem/FAT/BiosParameterBlock.h"
#include "../filesystem/FAT/DirectoryEntry.h"
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
                printf("Kernel v%d\n", 0);

            }
            if(strncmp("CLEAR", command, characterCount) == 0)
            {
                kterm_init();
                printf("|===    BarinkOS       ===|\n");
            }
            if(strncmp("FAT", command, characterCount) == 0)
            {

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


                // FIXME: If no drive is connected we continue trying to read from
                // a not connected drive! 
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

                auto*  bpb =  (BiosParameterBlock*) biosparameterblock;
                

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
                for(unsigned short i : FAT) {
                    printf("%x ", i);
                }
                kterm_put('\n');


                uint32_t RootDirectoryRegion = FATAddress + ( bpb->NumberOfFileAllocationTables * bpb->NumberOfSectorsPerFAT );
                uint32_t DataRegion = RootDirectoryRegion + ((bpb->NumberOfDirectoryEntries * 32) / bpb->BytesPerSector );

                uint16_t data2 [256];
                ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, RootDirectoryRegion, data2 );
                auto* RootDirectory = (DirectoryEntry*) data2;
                // List files in root
                for(int i= 0; i < bpb->NumberOfDirectoryEntries ; i++ )
                {  
                auto* entry = (DirectoryEntry*)((uint32_t) RootDirectory + (i  * sizeof(DirectoryEntry)));

                if( entry->filename[0] == (uint8_t) 0x00 )
                    break; // There are no more entries in this directory or the entry is free
                    
                if( (entry->attribute & 0x01) == 0x01 || (entry->attribute & 0x20) == 0x20)
                    continue; // Skip listing if hidden or Achieve flag is set

                // Print the filename;
                for(char n : entry->filename){
                    if(n == 0x20)
                        break;
                    kterm_put(n);
                }
                kterm_put('\n');

                for(unsigned char n : entry->Extension){
                    kterm_put(n);
                }
                kterm_put('\n');

                printf("Attribute: %x \n" , entry->attribute);
                printf("FileSize: %d Bytes\n", entry->FilesizeInBytes);

                if( entry->FilesizeInBytes != 0x0 || (entry->attribute & 0x8) == 0x0){
                    printf("Show contents");

                    printf( "Start cluster of the file: 0x%x\n" , entry->StartingCluster);

                    printf("IS it only 1 cluster? %s\n" , FAT[i] == 0xFFFF? "Yes": "No" );

                    uint32_t sector = DataRegion + ((entry->StartingCluster - 0x02 ) * bpb->SectorsPerCluster);


                    uint16_t dataBlob [256];
                    ATA_DEVICE::Read(BUS_PORT::Primary, DEVICE_DRIVE::MASTER, sector, dataBlob );
                    for(unsigned short n : dataBlob)
                    {
                        kterm_put(n & 0x00ff);

                        kterm_put(n >> 8);
                    }kterm_put('\n');


                }

                printf("======================\n");
                

                }
                continue;
            }



            if(strncmp("DEVICES", command, characterCount) == 0){
                printf("================ CONNECTED DEVICES ===============\n");






                
            }


            printf("executed command: %s\n", command);


            delay(1000);
        }   
}