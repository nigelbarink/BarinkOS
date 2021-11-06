#pragma once

#include "../bootloader/multiboot.h"
#include <stdint.h>
#include <stddef.h>
#include "../../libc/include/mem.h"
#include "../kstructures/bitmap.h"


#define BLOCK_SIZE 4092
#define BLOCKS_PER_WORD 32

#define KB_TO_BLOCKS(x) (((x) * 1024 ) / BLOCK_SIZE)
#define IS_ALIGNED(addr, align) !((addr) & ~((align) - 1))
#define ALIGN(addr, align) (((addr) & ~((align) - 1 )) + (align))


extern void PhysicalMemoryManager_initialise(uint32_t, size_t);

extern void PhysicalMemoryManager_region_initialise(uint32_t, size_t);
extern void PhysicalMemoryManager_region_deinitialise(uint32_t,size_t);

extern void PhysicalMemoryManager_initialise_available_regions(uint32_t, uint32_t);
extern void PhysicalMemoryManager_deinitialise_kernel();

extern void* PhysicalMemoryManager_allocate_block();
extern void PhysicalMemoryManager_free_block(void* p);


extern size_t mem_size;
extern int used_blocks;
extern size_t max_blocks;
extern uint32_t* pmmap;
extern size_t pmmap_size ; 