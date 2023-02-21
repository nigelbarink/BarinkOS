#include "ataDevice.h"
#include "../../io/io.h"

#define IS_BIT_SET(x, bit) ((x >> bit & 0x1) == 1)

void ATAPIO::Read(uint16_t DEVICE_CHANNEL,  DEVICE_DRIVE drive, uint32_t LBA, uint16_t* buffer) {
    /*
    Assume you have a sectorcount byte and a 28 bit LBA value. A sectorcount of 0 means 256 sectors = 128K.

    Notes:  - When you send a command byte and the RDY bit of the Status Registers is clear, you may have to wait (technically up to 30 seconds) for the drive to spin up, before DRQ sets. You may also need to ignore ERR and DF the first four times that you read the Status, if you are polling.
            - for polling PIO drivers: After transferring the last uint16_t of a PIO data block to the data IO port, give the drive a 400ns delay to reset its DRQ bit (and possibly set BSY again, while emptying/filling its buffer to/from the drive).
            - on the "magic bits" sent to port 0x1f6: Bit 6 (value = 0x40) is the LBA bit. This must be set for either LBA28 or LBA48 transfers. It must be clear for CHS transfers. Bits 7 and 5 are obsolete for current ATA drives, but must be set for backwards compatibility with very old (ATA1) drives.
    
    An example of a 28 bit LBA PIO mode read on the Primary bus:
    
    */
    
    const int sectorCount = 1;

    // Floating bus check
    uint8_t floatingBus  = inb(DEVICE_CHANNEL | 7);
    if (floatingBus == 0xFF){
        printf("Floating bus!!");
        return ;
    }

    //printf("Read LBA: 0x%x\n", LBA);
    // Send 0xE0 for the "master" or 0xF0 for the "slave", ORed with the highest 4 bits of the LBA to port 0x1F6: outb(0x1F6, 0xE0 | (slavebit << 4) | ((LBA >> 24) & 0x0F))
    outb(DEVICE_CHANNEL | 6 , ( 0xE0 | (LBA >>28) ) );
    // Send a NULL byte to port 0x1F1, if you like (it is ignored and wastes lots of CPU time): outb(0x1F1, 0x00)
    outb(DEVICE_CHANNEL | 1, 0x0000 );
    //Send the sectorcount to port 0x1F2: outb(0x1F2, (unsigned char) count)
    outb(DEVICE_CHANNEL | 2, sectorCount);
    //Send the low 8 bits of the LBA to port 0x1F3: outb(0x1F3, (unsigned char) LBA))
    outb(DEVICE_CHANNEL | 3, LBA);
    //Send the next 8 bits of the LBA to port 0x1F4: outb(0x1F4, (unsigned char)(LBA >> 8))
    outb(DEVICE_CHANNEL | 4, (LBA >> 8));
    //Send the next 8 bits of the LBA to port 0x1F5: outb(0x1F5, (unsigned char)(LBA >> 16))
    outb(DEVICE_CHANNEL | 5, (LBA >> 16));
    //Send the "READ SECTORS" command (0x20) to port 0x1F7: outb(0x1F7, 0x20)
    outb(DEVICE_CHANNEL | 7, 0x20);

    volatile int i,j;
        for(i=0;i<2000;i++)
            for(j=0;j<25000;j++)
                asm("NOP");

    //Wait for an IRQ or poll.
    uint8_t status = inb(DEVICE_CHANNEL | 7);
    if( status == 0x00){
        printf("No drive\n");
        return;
    }

    //printf("Status: %x\n", status);
    // Check if busy!
    while((status & 0x80) == 0x80){
        printf("Reading....\r");
        status = inb(DEVICE_CHANNEL | 7);
    }
    
    
    if ((status & 0x01) == 0x01){
        printf("Error occured during read!\n");
        return;
    }

    //Transfer 256 16-bit values, a uint16_t at a time, into your buffer from I/O port 0x1F0.    
     if( status & 0x01){
        printf("Error!\n");
        printf("Status: 0x%x\n", status);
        uint16_t error_register = inb(DEVICE_CHANNEL | 1);
        printf("Error register 0x%x\n",error_register );
        return ;
    }
    for ( int i = 0; i < 256; i++){
        uint16_t data;
        asm volatile ("inw %1, %0" : "=a"(data): "Nd"(DEVICE_CHANNEL));
       // printf (" %x ", data);

        buffer[i] = data;
    }
    
    //Then loop back to waiting for the next IRQ (or poll again -- see next note) for each successive sector.

}

void ATAPIO::Write(uint16_t data, DEVICE_DRIVE dev){

    printf("Not implemented\n");

}

void ATAPIO::Identify(uint16_t DEVICE_CHANNEL,DEVICE_DRIVE drive ){
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


    //printf("channel selected: 0x%x", DEVICE_CHANNEL);
    // Assuming Master here
    // Select the target drive
    outb(DEVICE_CHANNEL | 6, drive); // on the primary bus select the master drive
    outb(DEVICE_CHANNEL | 6 , 0x0); // write 0 to the controlport for some reason

    outb(DEVICE_CHANNEL | 6, drive);
    uint8_t status = inb(DEVICE_CHANNEL | 7 );
    if(status == 0x00){
        printf("No drive\n");
        return;
    }
    // send the identify command;
    outb(DEVICE_CHANNEL | 7, 0xEC);


    // set the sectorCount, LBAlo, LBAmid, LBA,hi IO ports to 0
    outb(DEVICE_CHANNEL | 2, 0);

    outb(DEVICE_CHANNEL | 3, 0);

    outb(DEVICE_CHANNEL | 4, 0);

    outb(DEVICE_CHANNEL | 5, 0);

    // send the identify command ;
    //printf("command sent!\n");
    outb(DEVICE_CHANNEL | 7 , 0xEC);

    // read the status port
    uint8_t status2 = inb(DEVICE_CHANNEL | 7);
    if( status2 == 0x00){
        printf("No drive\n");
        return;
    }

    //printf("Waiting until ready...\n");
    while(((status2 & 0x80 == 0x80)
           && (status2 & 0x01) != 0x01)
            ) status2 = inb(DEVICE_CHANNEL | 7);

    if( status2 & 0x01){
        printf("Error!\n");
        return ;
    }

    uint16_t deviceIdentify [256] = {0};

    for ( int i = 0; i < 256; i++){
        uint16_t data;
        asm volatile ("inw %1, %0" : "=a"(data): "Nd"(DEVICE_CHANNEL));

        deviceIdentify[i] = data;
    }


    printf("Model-label (ASCII hex): ");
    for(int i = 27; i < 47; i++){
        kterm_put((char)(deviceIdentify[i] >> 8));
        kterm_put((char)(deviceIdentify[i] & 0x00FF));
    }
    kterm_put('\n');

}

void ATAPIO::Soft_Reset(uint8_t DEVICE_CHANNEL,DEVICE_DRIVE drive){
    printf("Soft reseting drive...\n");
   // outb(channels[DEVICE_CHANNEL].base + 7 , 0x4);
    // wait a bit..
    for(int i = 0 ; i < 1000000; i++){
        asm volatile("NOP");
    }
    //outb(channels[DEVICE_CHANNEL].base + 7 , 0x0);

}



