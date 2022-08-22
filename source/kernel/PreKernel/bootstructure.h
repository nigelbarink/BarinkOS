#pragma once 
#include <stddef.h>
#include <stdint.h>


extern "C" const uint32_t kernel_begin;
extern "C" const uint32_t kernel_end;
struct BootInfoBlock {
    bool MapIsInvalid;
    uint32_t bootDeviceID ;

    uint32_t GrubModuleCount;

    bool ValidSymbolTable;
    uint32_t SymbolTableAddr;
    uint32_t SymbolTabSize;
    uint32_t SymbolStrSize;

    bool ValidELFHeader;

    bool EnabledVBE;

    bool PhysicalMemoryMapAvailable;

};

// Put the BootInfoBlock 1MB above the kernel.
const uint32_t BootInfoBlock_pptr = kernel_end - 0xC0000000 + 0x1000;
