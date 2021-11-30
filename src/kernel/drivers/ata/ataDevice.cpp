#include "atapiDevice.h"
#define IS_BIT_SET(x, bit) ((x >> bit & 0x1) == 1) 

void ATA_DEVICE::Soft_Reset(uint8_t DEVICE_CHANNEL,DEVICE_DRIVE drive){
        printf("Soft reseting drive...\n");
        outb(channels[DEVICE_CHANNEL].base + 7 , 0x4);
        // wait a bit..
        for(int i = 0 ; i < 1000000; i++){
            asm volatile("NOP");
        }   
        outb(channels[DEVICE_CHANNEL].base + 7 , 0x0);

}


void ATA_DEVICE::Identify(uint8_t DEVICE_CHANNEL,DEVICE_DRIVE drive ){
    // lets ignore which port we actually want to check for now !

    /*
        THE STEPS INVOLVED

        1. Select the target drive by sending master (0x0A) or slave (0x0B) to the 
        drive select IO port

        2. Set the Sectorcount, LBAlo, LBAmid and LBAhi IO ports to 0 

        3. Send the identify command (0xEC) to the command IO port 

        4. Read the status port 
            4.2 If the value is 0x0 the drive does not exist
            4.3 If it has any other value continue
        5. poll the status port until bit 7 is clear.
        6. Check if the LBAmid and LBAhi ports are non-zero
            6.2. If non-zero stop polling this is not an ATA device
            6.3 If zero continue
        
        7. poll status port until bit 3 is set or bit 0 is set

        8. if err is clear, read the data from the data port 


    */


    // Assuming Master here 
    // Select the target drive
    outb(0x176, 0x0A); // on the primary bus select the master drive

    // Set the Sectorcount, LBAlo, LBAmid and LBAhi IO ports to 0
    outb(0x172, 0); 

    outb(0x173, 0); 

    outb(0x174, 0); 

    outb(0x175, 0);

    // send the identify command;
    outb(0x177, 0xA1);

    // read the status port
    uint8_t status = inb(0x177);
    if( status == 0x00){
        printf("No drive\n");
        return;
    }

    while(true){
        status = inb(0x177);

        if( status & (~8))
            break;
    }

    printf("Is this an ATA device?\n");
    uint8_t LBAmid = inb(0x174); 
    uint8_t LBAhi = inb(0x175);

    printf("LBAmid: 0x%x, LBAhi: 0x%x\n", LBAmid, LBAhi);


    if( LBAhi != 0x0 || LBAmid != 0x0){
        printf("Not ATA device.. Stopping..\n");
       // return;
    } 

    while(true){
        status = inb(0x177);
        printf( "Status bit: 0x%x\n", status);
        if ( IS_BIT_SET(status, 3)){
            printf("Status: ready!\n");
            break;

        }

        if( IS_BIT_SET(status, 0)){
            printf("Status: error!\n");
            break;
        }
    }

    if( IS_BIT_SET(status, 0) == false){
        // READ DATA

        uint16_t deviceIdentify [256];

        for (int i= 0; i < 256; i++){
            uint8_t data = inb(0x170);
            uint8_t data2 = inb(0x170);
            
            deviceIdentify[i] = (uint16_t) (  (uint16_t) data  << 8  | (uint16_t) data2 );

        }


        printf("Data received!\n");
    }

    printf("Error bit was set!\n");


}


void ATA_DEVICE::Read(uint8_t DEVICE_CHANNEL,  DEVICE_DRIVE drive) {
    printf("Not implemented");
}

void ATA_DEVICE::Write(uint8_t DEVICE_CHANNEL, DEVICE_DRIVE drive) {
    printf("Not implemented");
}