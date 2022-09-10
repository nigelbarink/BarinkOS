#pragma once
#include <stdint.h> 
#include "../pci/pciDevice.h"
#include "../tty/kterm.h"
#include "ideCommands.h"
#include "sampleIDE.h"

#define IS_BIT_SET(x, bit) ((x >> bit & 0x1) == 1) 

IDEChannelRegisters channels[2];
IDE_DEVICE ide_devices[4];

inline void CheckProgIF(uint8_t ProgIF){
    if( IS_BIT_SET(ProgIF, 0) ) // Is the 0th bit set
    {
        printf ("Primary Channel is in PCI native mode\n");
    } else{
        printf("Primary Channel is in Compatibility mode\n");
    }

    if( IS_BIT_SET(ProgIF, 1)){
        printf("Bit 0 can be modified\n");
    }else{
        printf("Bit 0 cannot be modified\n");
    }
    
    if( IS_BIT_SET(ProgIF, 2)){
        printf("Secondary channel is in PCI native mode\n");
    }else{
        printf("Secondary channel is in Compatibility mode\n");
    }
    
    if( IS_BIT_SET(ProgIF, 3)){
        printf("Bit 2 can be modified\n");
    }else{
        printf("Bit 2 cannot be modified\n");
    }


    if( IS_BIT_SET(ProgIF , 7)){
        printf("This is a bus master IDE Controller\n");
    } else{
        printf("This controller doesn't support DMA!\n");
    }

}

inline void TestIDEController(){
    // Do stuff
    printf("Testing IDE controllers\n");

    // NOTE: Testing done with a hard coded known PCI addres 
    // Of an intel PIIX3 IDE Controller
    int bus = 0;
    int device =1  , function = 1;
    PCIBusAddress IDEControllerPCIAddress = PCIBusAddress{bus,device, function};

    uint8_t ProgIF = GetProgIF(IDEControllerPCIAddress);
    printf( "ProgIF: 0x%x\n" ,ProgIF);

    //CheckProgIF(ProgIF);

    // For this test will just assume all bits are set 
    // the CheckProgIF can check but on the test machine all bits are set anyways

    uint32_t BAR0,BAR1,BAR2,BAR3, BAR4;

    BAR0 = ReadBAR(IDEControllerPCIAddress, 0); 

    BAR1 = ReadBAR(IDEControllerPCIAddress, 1);

    BAR2 = ReadBAR(IDEControllerPCIAddress, 2);

    BAR3 = ReadBAR(IDEControllerPCIAddress, 3);

    BAR4 = ReadBAR(IDEControllerPCIAddress, 4);

    // All bars are return 0xffffff for some as of yet mysterious reason!
    printf( "BAR 0: 0x%x\n", BAR0);

    printf( "BAR 1: 0x%x\n", BAR1);

    printf( "BAR 2: 0x%x\n", BAR2);

    printf( "BAR 3: 0x%x\n", BAR3);

    printf( "BAR 4: 0x%x\n", BAR4);

    init_IDE(BAR0, BAR1, BAR2, BAR3, BAR4);
    
    // Read Something from disc
    unsigned int maxByteCount = 20 ;
    void* MDA_buffer = (void*)0xC0000000;



}
