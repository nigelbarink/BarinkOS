#pragma once 
#include <stddef.h>
#include <stdint.h>

extern "C" const uint32_t kernel_begin;
extern "C" const uint32_t kernel_end;

#define IS_AVAILABLE_MEM(MEM_TYPE) MEM_TYPE & 0x1
#define IS_ACPI_MEM(MEM_TYPE) MEM_TYPE & 0x2
#define IS_RESERVED_MEM(MEM_TYPE) MEM_TYPE & 0x4
#define IS_NVS_MEMORY(MEM_TYPE) MEM_TYPE & 0x8
#define IS_BADRAM_MEMORY(MEM_TYPE) MEM_TYPE & 0x10
           
struct MemoryInfoBlock {
    uint32_t Base_addr ;
    uint32_t Memory_Size;
    MemoryInfoBlock* next;
    uint8_t  type;

};
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
    MemoryInfoBlock* MemoryMap;
    uint32_t map_size;
    uint32_t MemorySize ;

};


const uint32_t pke = ((uint32_t)&kernel_end) - 0xC0000000;
const uint32_t BootInfoBlock_pptr = pke + 1000   - sizeof(BootInfoBlock);
const uint32_t MemoryMapHeap_pptr = pke + 0x1;
