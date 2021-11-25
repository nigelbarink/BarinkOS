#include "pci.h"
#include "tty/kterm.h"
#define PCI_BUS_ADDR_SHIFT 16
#define PCI_DEVICE_ADDR_SHIFT  11
#define PCI_FUNCTION_ADDR_SHIFT 8
#define PCI_ENABLE_ADDR_SHIFT 31



uint32_t ConfigReadWord (uint8_t bus, uint8_t device, uint8_t func, uint8_t offset){
    uint32_t address;

    address = (uint32_t) (
        ((uint32_t) 1 << PCI_ENABLE_ADDR_SHIFT) |
        ((uint32_t)bus << PCI_BUS_ADDR_SHIFT) |
        ((uint32_t)device << PCI_DEVICE_ADDR_SHIFT) |
        ((uint32_t)func << PCI_FUNCTION_ADDR_SHIFT) | 
        offset );
   // printf("PCI address read 0x%x", address);

    

    outl(CONFIG_ADDRESS, address);

    
    return inl(CONFIG_DATA);
}

