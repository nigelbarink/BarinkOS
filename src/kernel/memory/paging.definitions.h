#pragma once
#include <stdint.h>
/*
    This file contains some handy definitions for different types 
    that have to do with paging in atleast 32 bit and maybe sometime in the future
    also 64 bit mode.
*/



#define MAX_DIRECTORY_ENTRIES 1024
#define MAX_PAGE_TABLE_ENTRIES 1024
#define MAX_PAGES 1024

#define PHYSICAL_ADDRESS uint32_t 
#define VIRTUAL_ADDRESS uint32_t

#define PageDirectoryEntry uint32_t
#define PageTableEntry uint32_t




// NOTE: FIXME: I am fairly certain these masks are off by one!

#define PD32_PRESENT_MASK                   (0x1 << 0)
#define PD32_READ_WRITE_MASK                (0x1 << 1)
#define PD32_SUPERVISOR_MASK                (0x1 << 2)
#define PD32_WRITE_THROUGH_MASK             (0x1 << 3)
#define PD32_CACHE_DISABLE_MASK             (0x1 << 4)
#define PD32_ACCESSED_MASK                  (0x1 << 5)
#define PD32_AVAILABLE_1_4KB_MASK           (0x1 << 6)
#define PD32_DISABLE_4MB_MASK               (0x1 << 6)
#define PD32_PAGE_SIZE_MASK                 (0x1 << 7)
#define PD32_GLOBAL_4MB_MASK                (0x1 << 8)
#define PD32_AVAILABLE_2_4MB_MASK           ( 14 << 9)
#define PD32_AVAILABLE_2_4KB_MASK           ( 15 << 8)
#define PD32_ADDRESS_4KB_MASK               (0x8FFFF << 12)
#define PD32_PAGE_ATTRIBUTE_TABLE_4MB_MASK  (0x1 << 12)
#define PD32_HIGH_HALF_ADDRESS_4MB_MASK     (0x7F<< 13)
#define PD32_RESERVED_4MB_MASK              (0x1 << 21)
#define PD32_LOWER_HALF_ADDRESS_4MB_MASK    (0x1FF << 22)

#define PT32_PRESENT_MASK                   (0x1 << 0)
#define PT32_READ_WRITE_MASK                (0x1 << 1)
#define PT32_SUPERVISOR_MASK                (0x1 << 2)
#define PT32_WRITE_THROUGH_MASK             (0x1 << 3)
#define PT32_CACHE_DISABLE_MASK             (0x1 << 4)
#define PT32_ACCESSED_MASK                  (0x1 << 5)
#define PT32_DIRTY_MASK                     (0x1 << 6)
#define PT32_PAGE_ATTRIBUTE_TABLE_MASK      (0x1 << 7)
#define PT32_GLOBAL_MASK                    (0x1 << 8)
#define PT32_AVAILABLE_MASK                 (0x7 << 9)
#define PT32_CACHE_DISABLE_MASK             (0x7FFFF << 12)

