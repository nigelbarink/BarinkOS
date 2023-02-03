#include "atapiDevice.h"
#define IS_BIT_SET(x, bit) ((x >> bit & 0x1) == 1) 

bool isPacketDevice(){
 
    uint8_t LBAmid = inb(0x174);
    uint8_t LBAhi = inb(0x175);

    printf(" LBAmid: 0x%x, LBAhi: 0x%x");
    return LBAmid == 0x14 && LBAhi == 0xEB;

}


void ATAPI_DEVICE::Identify(uint8_t DEVICE_CHANNEL,DEVICE_DRIVE drive ){
    // lets ignore which port we actually want to check for now !

    /*   THE STEPS INVOLVED
      
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

    // Select the target drive
    outb(0x176, 0xA0); // on the secondary bus select the master drive
    outb(0x170 + 0x206 , 0x0); // write 0 to the controlport for some reason

    outb(0x176, 0xA0);
    // read the status port
    uint8_t status = inb(0x177);
    printf("status after drive select: 0x%x\n",status);
    if( status == 0x00){
        printf("No drive\n");
        return;
    }

    outb(0x176, 0xA0);


    // Set the Sectorcount, LBAlo, LBAmid and LBAhi IO ports to 0
    outb(0x172, 0); 

    outb(0x173, 0); 

    outb(0x174, 0); 

    outb(0x175, 0);

    // send the identify command;
    printf("command sent!\n");
    outb(0x177, 0xA1);

    // read the status port
    uint8_t status2 = inb(0x177);
    if( status2 == 0x00){
        printf("No drive\n");
        return;
    }


    printf("Waiting until ready...\n");

    while(((status2 & 0x80 == 0x80)
        && (status2 & 0x01) != 0x01)
     ) status2 = inb(0x177);
       

    if(status2  & 0x01){
        printf("Error!");
        return;
    }
   

    // READ DATA

    uint16_t deviceIdentify [256] ={0};

    for (int i= 0; i < 256; i++){
           uint16_t data;
            asm volatile ( "in %1, %0"
                   : "=a"(data)
                   : "Nd"(0x170) );

        
        deviceIdentify[i] =  data ;
        

    }
    
    printf("Model-label (ASCII hex):\n");
    for(int i = 27; i < 47; i++){
        printf(" %x ",deviceIdentify[i]);
    }

    printf("\nSerial number (ASCII hex):\n");
    for (int i = 10; i < 19; i++){
        printf(" %x ", deviceIdentify[i]);
    }

    printf("\nFirmware revision (ASCII hex):\n");
    for (int i = 23; i < 26; i++){
        printf(" %x ", deviceIdentify[i]);
    }

    printf("\nConfiguration: %x\n", deviceIdentify[0]);

   

    printf("\nData received!\n");

    
  



}


void ATAPI_DEVICE::Read(uint8_t DEVICE_CHANNEL,  DEVICE_DRIVE drive) {
    printf("Not implemented");
}

void ATAPI_DEVICE::Write(uint8_t DEVICE_CHANNEL, DEVICE_DRIVE drive) {
    printf("Not implemented");
}


