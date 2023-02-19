#pragma once
#include <stddef.h>
#include <CoreLib/Memory.h>

#include "../prekernel/bootstructure.h"
#include "../terminal/kterm.h"
#include "../bitmap.h"

#define BLOCK_SIZE 4092 


void SetupPhysicalMemoryManager(uint32_t mapAddress, uint32_t memorySize);

void* allocate_block();
void free_block(void* ptr);

void allocate_region(uint32_t address, uint32_t size);
void deallocate_region(uint32_t address, uint32_t size);

int GetUsedBlocks();