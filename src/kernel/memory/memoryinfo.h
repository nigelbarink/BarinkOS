#pragma once
#include <stdint.h>
#include <stddef.h>

struct MemoryArea{
    void* StartAddress;
    size_t Size;
    unsigned int type;
    MemoryArea* Next;

}__attribute__((packed));

struct MemoryInfo {
    uint32_t TotalMemory;
    uint32_t ReservedMemory;
    MemoryArea* MemoryRegionList;
}__attribute__((packed));



