#include "./PhysicalMemoryManager.h"
#define IS_ALIGNED(addr, align) !((addr) & ~((align) - 1))
#define ALIGN(addr, align) (((addr) & ~((align) - 1 )) + (align))

const uint32_t KERNEL_OFFSET = 0xC0000000;

uint32_t* memoryBitMap;
uint32_t pmmap_size;
uint32_t max_blocks;
int used_blocks;


void SetupPhysicalMemoryManager(uint32_t mapAddress, uint32_t memorySize ) 
{
     /*
        Every byte contains 8 pages
        A page is 4096 kib 
        Every block (1 bit) represent an page 
    */

    // Set the maximum number of blocks
    max_blocks = (uint32_t)memorySize / BLOCK_SIZE ;
    printf("Max Blocks: %d\n", max_blocks);
    
    // Set size of the bitmap 
    uint32_t bitmap_size = max_blocks / 32;
    printf("Bitmap size: %d bytes\n",bitmap_size);

    // Set blocks used to zero
    used_blocks = max_blocks;
    
    // set the address of the memory bitmap
    memoryBitMap = (uint32_t*) mapAddress;

    // Set all places in memory as free
    memset(memoryBitMap, 0xFFFFFFFF,  max_blocks / 32  ); 
}

// NOTE: This can only give blocks of 4kb at a time!
//         We might at some point want to allocate multiple blocks at once.
void* allocate_block() {
    uint8_t blocks_available = max_blocks - used_blocks;
    // Are there any blocks available?
    if( blocks_available <= 0)
    {
        printf("No blocks available. Blocks Delta: 0x%x\n", blocks_available);
        return 0;
    }

    // Find 1 free block somewhere
    int free_block_index = bitmap_first_unset(memoryBitMap, max_blocks / 8 );

    if(free_block_index == -1)
    {
        printf("Could not find a good block!\n");
        // Could not find a block
        return (void*)0xFFFF;
    }

    if(free_block_index == 0)
        printf("Somethings wrong!!!\n");

    // Set the block to be used!
    bitmap_unset(memoryBitMap, free_block_index);
    // Increase the used_block count!
    used_blocks++;
    printf("used blocks: 0x%x\n", used_blocks);
    // return the pointer to the physical address
    return (void*) (BLOCK_SIZE * free_block_index);
}

void free_block(void* p) {
    // If it is a null pointer we don't need to do anything.
    if(p==0) {
        return;
    }
    // calculate the index into the bitmap
    int index  = ((uint32_t) p) / BLOCK_SIZE;
    
    // set the block to be free
    bitmap_set(memoryBitMap, index);
    used_blocks--;
    printf("used blocks: 0x%x, after free\n", used_blocks);

}

void allocate_region(uint32_t startAddress, uint32_t size) {
    // every bit should be 4KiB 
    // every byte is 8*4KiB = 32KiB
    
    int NumberOfBlocksToAllocate = ( size / 1024) / 4  / 8 + 1;
    int startBlock = (startAddress / 1024) / 4 / 8 ;

    for( int i = 0; i < NumberOfBlocksToAllocate; i++)
    {
        bitmap_unset(memoryBitMap, startBlock + i);// allocate region causes #PF Exception
        used_blocks++;
    }
}

void deallocate_region(uint32_t  StartAddress , uint32_t size ) {
    // reverse of what happened in allocate_region
    int NumberOfBlocks = (size / 1024) / 4 / 8 + 1;
    int startBlock = (StartAddress / 1024) / 4 / 8;

    for(int i = 0; i < NumberOfBlocks; i++)
    {
        bitmap_set(memoryBitMap, startBlock + i);
        used_blocks --;
    }
}

int GetUsedBlocks (){
    return used_blocks;
}
