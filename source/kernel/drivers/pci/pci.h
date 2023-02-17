#pragma once
#include <stdint.h>
#include "../io/io.h"
#include "../../terminal/kterm.h"
#include "pciDevice.h"

// Configuration Space Access Mechanism #1
#define CONFIG_ADDRESS 0xCF8 // Configuration adress that is to be accessed
#define CONFIG_DATA 0xCFC // Will do the actual configuration operation
#define PCI_BUS_ADDR_SHIFT 16
#define PCI_DEVICE_ADDR_SHIFT  11
#define PCI_FUNCTION_ADDR_SHIFT 8
#define PCI_ENABLE_ADDR_SHIFT 31

class PCI  {
public:
    static void Scan();
    static uint32_t ConfigReadWord ( PCIBusAddress& PCIDeviceAddress , uint8_t offset);
    static uint8_t GetProgIF (PCIBusAddress& PCIDeviceAddress);
    static uint32_t ReadBAR ( PCIBusAddress& PCIDeviceAddress, int bar_number);
    static uint32_t ConfigReadWord (uint8_t bus, uint8_t device, uint8_t func, uint8_t offset);
    static uint8_t GetHeaderType( PCIBusAddress& PCIDeviceAddress );
    static uint16_t GetClassCodes( PCIBusAddress& PCIDeviceAddress );
    static bool IsMultiFunctionDevice(PCIBusAddress& PCIDeviceAddress);
    static uint64_t GetDevice (int bus, int device, int function );


    static const char* getClassName (uint8_t ClassCode);
    static const char* getVendor( uint32_t VendorID);
    static void PrintPCIDevice(PCIBusAddress& PCIDevice);

private:
    struct ClassCode {
        const char* name;
        uint8_t code;
    };
    static constexpr ClassCode ClassCodeNames []= {
            {"Unclassified", 0x0},
            {"MassStorage Controller", 0x1},
            {"Network Controller", 0x2},
            {"Display Controller", 0x3},
            {"Multimedia Controller", 0x4},
            {"Memory Controller", 0x5},
            {"Bridge", 0x6},
            {"Simple Communication Controller", 0x7},
            {"Base System Peripheral", 0x8},
            {"Input Device Controller", 0x9},
            {"Docking Station", 0xA},
            {"Processor", 0xB},
            {"Serial Bus Controller", 0xC},
            { "Wireless Controller", 0xD},
            {"Intelligent Controller", 0xE},
            {"Satellite Communication Controller", 0xF},
            {"Encryption Controller", 0x10},
            {"Signal Processing Controller", 0x11},
            { "Processing Accelerator", 0x12},
            { "Non-Essential Instrumentation", 0x13}
    };
    static const uint8_t KnownClassCodes = sizeof(ClassCodeNames) / sizeof(ClassCode);
};

