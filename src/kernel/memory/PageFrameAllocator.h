#pragma once
#include "../arch/i386/tty/kterm.h"

#include <stdint.h>
#include "../bootloader/multiboot.h"

struct MemoryInfo{
    uint32_t memorySizeInBytes = 0;
    uint32_t reservedMemoryInBytes = 0;
};


extern void *kernel_begin;
extern void *kernel_end;




void print_Multiboot_memory_Map(multiboot_memory_map_t*);
void mapMultibootMemoryMap(multiboot_info_t*);