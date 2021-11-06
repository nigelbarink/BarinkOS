#include "PhysicalMemoryManager.h"

size_t mem_size = 0;
int used_blocks = 0;
size_t max_blocks = 0;
uint32_t* pmmap = 0;
size_t pmmap_size = 0; 


void PhysicalMemoryManager_initialise(uint32_t physicalmemorymap_address, size_t size )
{
    mem_size = size;
    max_blocks = KB_TO_BLOCKS(mem_size);

    used_blocks = max_blocks;

    pmmap = (uint32_t*) physicalmemorymap_address;

    if(max_blocks % BLOCKS_PER_WORD)
        pmmap_size++;
    
    memset(pmmap, 0xFF, pmmap_size);
}

void PhysicalMemoryManager_region_initialise(uint32_t base, size_t size)
{
    size_t blocks = size /BLOCK_SIZE;
    uint32_t align = base / BLOCK_SIZE;

    for(size_t i = 0 ; i < blocks; i ++)
    {
        bitmap_unset(pmmap, align++);
        used_blocks--;
    }

    bitmap_set(pmmap, 0);

}

void PhysicalMemoryManager_region_deinitialise(uint32_t base, size_t size )
{
    size_t blocks = size / BLOCK_SIZE;
    uint32_t align = base / BLOCK_SIZE;

    for(size_t i = 0 ; i < blocks; i++ )
    {
        bitmap_set(pmmap, align++);
        used_blocks++;
    }


}

void PhysicalMemoryManager_initialise_available_regions(uint32_t mmap_, uint32_t mmap_end_)
{
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)mmap_;
    multiboot_memory_map_t *mmap_end= (multiboot_memory_map_t*) mmap_end_;

    for(int i = 0; mmap < mmap_end ; mmap++, i++)
    {   
        if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            PhysicalMemoryManager_region_initialise((uint32_t) mmap->addr, (size_t) mmap->len);
        }
    }

}

void PhysicalMemoryManager_deinitialise_kernel()
{
    extern uint8_t kernel_begin;
    extern uint8_t kernel_end;

    size_t kernel_size = (size_t) &kernel_end - (size_t) &kernel_begin;

    uint32_t pmmap_size_aligned = pmmap_size;
    if(!IS_ALIGNED(pmmap_size_aligned, BLOCK_SIZE))
    {
        pmmap_size_aligned = ALIGN(pmmap_size_aligned, BLOCK_SIZE);
    }
    
    PhysicalMemoryManager_region_deinitialise((uint32_t) &kernel_begin, kernel_size);
    PhysicalMemoryManager_region_deinitialise((uint32_t) &kernel_end, pmmap_size_aligned);
    
}

void* PhysicalMemoryManager_allocate_block()
{
    if(used_blocks - max_blocks <= 0)
    {
        return 0;
    }

    int p_index = bitmap_first_unset(pmmap, p_index );

    if(p_index == -1){
        return 0;
    }

    bitmap_set(pmmap, p_index);
    used_blocks++;

    return (void*) (BLOCK_SIZE * p_index);
}

void PhysicalMemoryManager_free_block(void* p){
    if(p==0){
        return ;
    }

    uint32_t p_addr = (uint32_t) p;

    int index  = p_addr / BLOCK_SIZE;
    bitmap_unset(pmmap, index);

    used_blocks--;

}