#pragma once
#include "Memory/memoryinfo.h"

struct BootInfo{
    const char* BootStructureID = "BarinkOS";
    MemoryInfo* memory;

}; 