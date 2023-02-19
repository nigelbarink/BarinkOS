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

};
