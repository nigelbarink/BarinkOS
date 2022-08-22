#pragma once 
#include <stddef.h>
//#include "../../multiboot.h"
#include "../memoryinfo.h"

void initialise_available_regions(uint32_t memoryMapAddr, uint32_t memoryMapLastAddr, uint32_t* memoryBitMap, int* used_blocks);

/*
void mapMultibootMemoryMap( MemoryInfo* memInfo , multiboot_info_t *mbt);
*/
/**
 * @brief Debug Verbose Functions
 * 
 * @param mmap 
 */
/*
void print_Multiboot_memory_Map(multiboot_memory_map_t* mmap);
*/
