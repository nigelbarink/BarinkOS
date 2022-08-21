#pragma once
#include <stdint.h>
#include <stddef.h>

#include "memoryinfo.h"
#include "../multiboot.h"
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

extern uint32_t kernel_begin;
extern uint32_t kernel_end;

void initialise_available_regions(uint32_t memoryMapAddr, uint32_t memoryMapLastAddr, uint32_t* memoryBitMap, int* used_blocks);

extern uint32_t* memoryBitMap;

class PhysicalMemory
{
    public:
        void setup(MemoryInfo* memory);
        void destroy();
        void free_block(void* ptr);
        void* allocate_block();
        void allocate_region(uint32_t, uint32_t);
        void deallocate_region(uint32_t   , uint32_t );
    
    private:
        size_t pmmap_size;
        size_t max_blocks;
        int used_blocks;
};

void mapMultibootMemoryMap( MemoryInfo* memInfo , multiboot_info_t *mbt);

/**
 * @brief Debug Verbose Functions
 * 
 * @param mmap 
 */
void print_Multiboot_memory_Map(multiboot_memory_map_t* mmap);
