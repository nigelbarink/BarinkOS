#pragma once
#include "memory/memoryinfo.h"


struct BootInfo{
    const char* BootStructureID = "BarinkOS";
    MemoryInfo* memory;

}; 