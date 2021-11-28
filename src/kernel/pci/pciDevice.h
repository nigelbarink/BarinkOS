#pragma once 
#include <stdint.h>

/*
* PCI devices API 
*/
struct PCIBusAddress{

    int bus ;
    int device ;
    int function[8];


    uint32_t getAddress( int deviceFunction = 0 ){
        return ((uint32_t) 1 << 31) |
                ((uint32_t) bus << 16) |
                ((uint32_t) device << 11)|
                ((uint32_t) function[deviceFunction] << 8) |
                0x0000;

    };
};

class PCIDevice {
    public : 
         PCIDevice (PCIBusAddress* , int );
         ~PCIDevice();
         PCIBusAddress const PCIAddress();

    private:
        int bus;
        int device;
        int function;
        int headerType;

};
