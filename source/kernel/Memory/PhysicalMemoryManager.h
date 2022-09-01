#pragma once
#include <stddef.h>
#include "../PreKernel/bootstructure.h"
#include "../Terminal/kterm.h"
#include "../Lib/mem.h"
#include "../bitmap.h"

// Asumming i386 for now!
#define BLOCK_SIZE 4092 

#define IS_ALIGNED(addr, align) !((addr) & ~((align) - 1))
#define ALIGN(addr, align) (((addr) & ~((align) - 1 )) + (align))

struct PhysicalMemoryManagerInfoBlock
{
    uint32_t* memoryBitMap;
    size_t pmmap_size;
    size_t max_blocks;
    int used_blocks;
};

void SetupPhysicalMemoryManager(BootInfoBlock* memory);
void free_block(void* ptr);
void* allocate_block();
void allocate_region(uint32_t, uint32_t);
void deallocate_region(uint32_t   , uint32_t );
