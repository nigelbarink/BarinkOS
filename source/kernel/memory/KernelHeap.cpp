#include "KernelHeap.h"
#include "VirtualMemoryManager.h"
extern "C" const uint32_t kernel_end;
// Size of heap meta data is 5 bytes 
struct heap_block{
    uint8_t Used;
    uint32_t Size;
};

uint32_t heap_size;
heap_block* start ;

void* malloc(size_t size)
{
    printf("Received request for %d bytes of memory\n", size);
    heap_block* current = start;

    // look for a free block
    while(current <  start + heap_size)
    {
        if(current->Size >= size && current->Used == false )
        {
            // We found a spot 
            printf("Block found!\n");

            // Set the spot to in-use 
            current->Used = true;


            // split the block 
            printf("Split block.\n");
            uint32_t oldSize = current->Size;
            current->Size = size;

            heap_block* new_block = current + sizeof(heap_block) + current->Size;
            new_block->Size = oldSize - ( sizeof(heap_block) + size);
            new_block->Used = false;

            // return the free address 
            // NOTE: added an offset from the initial address to accomodate for 
            // meta-data.
            return current + sizeof(heap_block);

        }

        current += current->Size + sizeof(heap_block);
    }   

    // If we are here we need more memory so we should 
    // probably ask the VMM for more 
    // TODO: ask for more memory | Extend kernel heap
    
    printf("ERROR: OUT OF HEAP MEMORY CONDITION IS NOT IMPLEMENTED. HEAP NEEDS TO BE EXTENDED!\n");
}

void free(void* addr)
{
    // clear the free boolean that corresponds to this adddress
    // This should be fairly simple
    heap_block* allocatedBlock = (heap_block*)(addr - sizeof(heap_block));
    allocatedBlock->Used = false;
}

void initHeap()
{
    void* HEAP_ADDRESS = allocate_block();
    printf("0x%x HEAP Paddr\n", HEAP_ADDRESS);
 
    Immediate_Map((uint32_t)HEAP_ADDRESS + 0xC0000000, (uint32_t)HEAP_ADDRESS );
    start = (heap_block*) ((uint32_t)HEAP_ADDRESS + 0xC0000000);
    heap_size = 4096;

    printf("Clear heap\n");
    // Clear the heap 
    printf("set at 0x%x %d bytes to zero\n", start , heap_size);

    memset((void*)start, 0x00,  heap_size /4);


    printf("Init first heap block\n");
    // initialzie 
    start->Size = heap_size - sizeof(heap_block);

    start->Used = false;

}