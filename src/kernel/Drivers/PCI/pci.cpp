#include "pci.h"

uint16_t ConfigReadWord (uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset){
    uint32_t address;
    uint32_t lbus = (uint32_t) bus;
    uint32_t lslot = (uint32_t) slot;
    uint32_t lfunc = (uint32_t) func;
    uint16_t tmp = 0;

    /* Create configuration address as per Figure 1 */
    address = (uint32_t) ((lbus << 16) |  (lslot << 11) | (lfunc << 8) | (offset & 0xFC) |((uint32_t) 0x80000000) );
    /*write out the address */ 
    outl(CONFIG_ADDRESS, address);
    /* read in the data */
    /* (offset & 2 ) * 8 ) = o will choosse the first word of the 32 bits register*/
    tmp = (uint16_t)((inl(CONFIG_DATA)) >> ((offset & 2) * 8) & 0xFFFF);
    return (tmp);
}

uint16_t CheckVendor (uint8_t bus, uint8_t slot) {
    uint16_t vendor, device;
    /* 
        Try and read the first configuration register. Since there ar no
        vendors that == 0xFFFF, it must be a non-existent device.
    */
   if((vendor = ConfigReadWord(bus, slot, 0,0)) != 0xFFFF) {
       device = ConfigReadWord(bus, slot, 0,2);
        // Possible read more config values ...
   } return (vendor);
}

void checkDevice (uint8_t bus, uint8_t device ) {
    uint8_t function = 0;

    uint16_t vendorID = CheckVendor(bus, device);
    if (vendorID == 0xFFFF) {
        return;
    }

    checkFunction (bus, device, function );
    headerType = getHeaderType(bus, device, function );
    if( (headerType & 0x80) != 0) {
        /* It is  a multi-function device, so check remaining functions */
        for (function = 1; function < 8; function++){
            if (CheckVendor(bus, device)!= 0xFFFF){
                checkFunction(bus, device, function );
            } 
        }
    }

}


void checkFunction (uint8_t bus, uint8_t device, uint8_t function ){
    uint8_t baseClass;
    uint8_t subClass;
    uint8_t secondaryBus;

    baseClass = getBaseClass(bus, device, function);
    subClass = getSubClass (bus, device, function );
    if ( (baseClass == 0x06) && (subClass == 0x04)){
        secondaryBus = getSecondaryBus(bus,device, function);
        checkBus(secondaryBus);
    }
}


// Brute-force scan
void checkAllBuses (){
    uint16_t bus;
    uint8_t device;

    for(bus = 0; bus < 256; bus++){
        for(device = 0; device < 32; device++){
            checkDevice(bus,device);
        }
    }
}

// Recursive scan
void checkBus (uint8_t bus){
    uint8_t device;

    for(device = 0; device < 32; device ++){
        checkDevice(bus,device);
    }
}

void checkAllBuses(){
    uint8_t function; 
    uint8_t bus;

    headerType = getHeaderType(0,0,0);
    if ( (headerType & 0x80) == 0 ){
        /* Single PCI host controller */
        checkBus(0);
    } else{
        /* Multiple PCI host controllers */
        for (function = 0; function < 8; function++){
            if( CheckVendor(0,0) != 0xFFFF) {
                break;
            }
            bus = function;
            checkBus(bus);
        }
    }

}