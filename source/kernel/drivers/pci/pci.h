#pragma once
#include <stdint.h>
#include "../io/io.h"
#include "../../terminal/kterm.h"
#include "pciDevice.h"

// Configuration Space Access Mechanism #1
#define CONFIG_ADDRESS 0xCF8 // Configuration adress that is to be accessed
#define CONFIG_DATA 0xCFC // Will do the actual configuration operation

extern const char* ClassCodeTable [0x13];



// Note: this could be used to make the api for receiving PCI class codes a bit 
// nicer.
struct ClassCodes {
    uint8_t ClassCode;
    uint8_t DeviceClass;
}__attribute__((packed));

uint32_t ConfigReadWord (uint8_t bus, uint8_t device, uint8_t func, uint8_t offset);
uint32_t ConfigReadWord ( PCIBusAddress& PCIDeviceAddress , uint8_t offset);

 inline uint64_t GetDevice (int bus, int device, int function ){
     return ConfigReadWord(bus, device, function,0x0);
 }

uint8_t GetHeaderType( PCIBusAddress& PCIDeviceAddress );

uint16_t GetClassCodes( PCIBusAddress& PICDeviceAddress );
const char* getVendor( uint64_t VendorID);
const char* GetClassCodeName (uint64_t ClassCode );

uint8_t GetProgIF (PCIBusAddress& PCIDeviceAddress);
void PCI_Enumerate();

uint32_t ReadBAR ( PCIBusAddress& PCIDeviceAddress, int bar_number);