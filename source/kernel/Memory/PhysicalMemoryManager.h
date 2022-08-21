#pragma once
#include <stddef.h>
#include "memoryinfo.h"

#include "../Terminal/kterm.h"
#include "../Lib/mem.h"
#include "../bitmap.h"

// Asumming 32 bit x86 for now!
#define BLOCK_SIZE 4092
#define WORD_SIZE 2 
#define BLOCKS_PER_WORD 32 

#define KB_TO_BLOCKS(x) (x  / BLOCK_SIZE)
#define IS_ALIGNED(addr, align) !((addr) & ~((align) - 1))
#define ALIGN(addr, align) (((addr) & ~((align) - 1 )) + (align))

struct PhysicalMemoryManagerInfoBlock
{
    uint32_t* memoryBitMap;
    size_t pmmap_size;
    size_t max_blocks;
    int used_blocks;
};

void initPMM(MemoryInfo* memory);
void free_block(void* ptr);
void* allocate_block();
void allocate_region(uint32_t, uint32_t);
void deallocate_region(uint32_t   , uint32_t );
