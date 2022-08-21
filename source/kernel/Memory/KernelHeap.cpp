#include "KernelHeap.h"

// Size of heap meta data is 5 bytes 
struct heap_block{
    uint8_t Used;
    uint32_t Size;
}

uint32_t heap_size;
heap_block* start ;

void* malloc(size_t size)
{
    printf("Received request for %d bytes of memory", size);
    heap_block* current = start;

    while(current <  start + heap_size)
    {
        if(current->size >= size && current->Used == false )
        {
            // We found a spot 
            // Set the spot to in-use 
            current->Used = false;
            // return the free address 
            // NOTE: added an offset from the initial address to accomodate for 
            // meta-data.
            return current + sizeof(heap_block);

        }

        current += current->Size + sizeof(heap_block);
    }   

    // If we are here we need more memory so we should 
    // probably ask the VMM for more 
    // TODO: ask for more memory
    
}

void free(void* addr)
{
    // clear the free boolean that corresponds to this adddress
    // This should be fairly simple
    heap_block* allocatedBlock = addr - sizeof(heap_block);
    allocate_block->Used = false;
}

void initHeap()
{
    
}