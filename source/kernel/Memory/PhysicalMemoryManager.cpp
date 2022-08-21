#include "./PhysicalMemoryManager.h"

PhysicalMemoryManagerInfoBlock* PMMInfoBlock;


void initPMM( MemoryInfo* memory) {

    // NOTE: Lets for now puts the Physical  memoryManagerBlock at a random address
    //      We'll think of a more proper solution a bit later
    PMMInfoBlock = (PhysicalMemoryManagerInfoBlock*) 0xCC900000;


    // calculate the maximum number of blocks
    PMMInfoBlock->max_blocks = KB_TO_BLOCKS(memory->TotalMemory);
    PMMInfoBlock->used_blocks = 0;
    PMMInfoBlock->memoryBitMap = (uint32_t*) 0xCCA00000;

    printf("Maximum Number of blocks: 0x%x, Number of bytes for memMap: 0x%x\n", PMMInfoBlock->max_blocks , (PMMInfoBlock->max_blocks/8));

    //Size of memory map 
    uint32_t memMap_size = (PMMInfoBlock->max_blocks / 8 ) ; 
    
    printf("Memory Map size:  0x%x\n", memMap_size );
    printf("size of int in bytes: 0x%x \n" , sizeof(int));

    // Set all places in memory as free
    memset(PMMInfoBlock->memoryBitMap, 0xFF, memMap_size  );
}

// NOTE: this can only give blocks of 4kb at a time!
void* allocate_block() {
    uint8_t blocks_available = PMMInfoBlock->max_blocks - PMMInfoBlock->used_blocks;
    // Are there any blocks available?
    if( blocks_available <= 0)
    {
        printf("No blocks available. Blocks Delta: 0x%x\n", blocks_available);
        return 0;
    }

    // Find 1 free block somewhere
    int free_block_index = bitmap_first_unset(PMMInfoBlock->memoryBitMap, (PMMInfoBlock->max_blocks /8) /*memMap Size*/ );


    
    if(free_block_index == -1)
    {
        printf("Could not find a good block!\n");
        // Could not find a block
        return (void*)0xFFFF;
    }

    if(free_block_index == 0)
        printf("Somethings wrong!!!\n");

    // Set the block to be used!
    bitmap_unset(PMMInfoBlock->memoryBitMap, free_block_index);
    // Increase the used_block count!
    PMMInfoBlock->used_blocks++;
    printf("used blocks: 0x%x\n", PMMInfoBlock->used_blocks);
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
    bitmap_set(PMMInfoBlock->memoryBitMap, index);
    PMMInfoBlock->used_blocks--;
    printf("used blocks: 0x%x, after free\n", PMMInfoBlock->used_blocks);

}

void allocate_region(uint32_t startAddress, uint32_t size) {
    // every bit should be 4KiB 
    // every byte is 8*4KiB = 32KiB
    
    int NumberOfBlocksToAllocate = ( size / 1024) / 4  / 8 + 1;
    int startBlock = (startAddress / 1024) / 4 / 8 ;
    
   // printf("NumberOfBlocksToAllocate: 0x%x\n", NumberOfBlocksToAllocate);
    //printf( "start block: 0x%x\n" , startBlock);
    for( int i = 0; i < NumberOfBlocksToAllocate; i++)
    {

        //printf("ALLOCATE BLOCK: 0x%x\n" , startBlock + i );
        bitmap_unset(PMMInfoBlock->memoryBitMap, startBlock+ i);
        PMMInfoBlock->used_blocks++;
    }


}
void deallocate_region(uint32_t  StartAddress , uint32_t size ) {
    // NOT IMPLEMENTED YET
}


