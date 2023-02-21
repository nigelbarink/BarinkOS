#pragma once 
#include <stdint.h>
/*
* PCI devices API 
*/
struct PCIBusAddress{

    int bus ;
    int device ;
    int function;


    uint32_t getAddress( ){
        return ((uint32_t) 1 << 31) |
                ((uint32_t) bus << 16) |
                ((uint32_t) device << 11)|
                ((uint32_t) function << 8) |
                0x0000;

    };
};

class PCIDevice {
    public : 
         PCIDevice (PCIBusAddress* , int );
         ~PCIDevice();
         PCIBusAddress const PCIAddress();


         inline const char* getDeviceString(){
             return "Not implemented";  //GetClassCodeName(deviceclass);
         }

        inline const char* getVendorString(){
            return "Not implemented"; // getVendor(VendorID);
        }

        inline void setVendorID (uint16_t id) {
            this->VendorID = id;
        }

    private:
        int bus;
        int device;
        int function;

        uint16_t VendorID;
        uint16_t DeviceID;
        uint8_t deviceclass;
        uint8_t devicesubclass;

        int headerType;

};
