#include "./memory.h"
uint32_t* memoryBitMap;
/*

*/
void PhysicalMemory::setup( MemoryInfo* memory) {

    // calculate the maximum number of blocks
    max_blocks = KB_TO_BLOCKS(memory->TotalMemory);

    used_blocks = 0;

    memoryBitMap = (uint32_t*) 0xCCA00000;


    printf("Maximum Number of blocks: 0x%x, Number of bytes for memMap: 0x%x\n", max_blocks , (max_blocks/8));

    //Size of memory map 
    uint32_t memMap_size = (max_blocks / 8 ) ; 
    
    printf("Memory Map size:  0x%x\n", memMap_size );
    printf("size of int in bytes: 0x%x \n" , sizeof(int));

    // Set all places in memory as free
    memset(memoryBitMap, 0xFF, memMap_size  );
}

// NOTE: this can only give blocks of 4kb at a time!
void* PhysicalMemory::allocate_block() {
    uint8_t blocks_available = max_blocks - used_blocks;
    // Are there any blocks available?
    if( blocks_available <= 0)
    {
        printf("No blocks available. Blocks Delta: 0x%x\n", blocks_available);
        return 0;
    }

    // Find 1 free block somewhere
    int free_block_index = bitmap_first_unset(memoryBitMap, (max_blocks /8) /*memMap Size*/ );


    
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


void PhysicalMemory::free_block(void* p) {
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



void PhysicalMemory::allocate_region(uint32_t startAddress, uint32_t size) {
    // every bit should be 4KiB 
    // every byte is 8*4KiB = 32KiB
    
    int NumberOfBlocksToAllocate = ( size / 1024) / 4  / 8 + 1;
    int startBlock = (startAddress / 1024) / 4 / 8 ;
    
   // printf("NumberOfBlocksToAllocate: 0x%x\n", NumberOfBlocksToAllocate);
    //printf( "start block: 0x%x\n" , startBlock);
    for( int i = 0; i < NumberOfBlocksToAllocate; i++)
    {

        //printf("ALLOCATE BLOCK: 0x%x\n" , startBlock + i );
        bitmap_unset(memoryBitMap, startBlock+ i);
        used_blocks++;
    }


}
void PhysicalMemory::deallocate_region(uint32_t  StartAddress , uint32_t size ) {
    // NOT IMPLEMENTED YET
}


void mapMultibootMemoryMap( MemoryInfo* memInfo , multiboot_info_t *mbt) {

    printf("mmap_addr = 0x%x, mmap_length = 0x%x\n", (unsigned) mbt->mmap_addr , (unsigned) mbt->mmap_length );


    multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) mbt->mmap_addr;

    for (;  (unsigned long) mmap <  mbt->mmap_addr + mbt->mmap_length;  mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size))){

            if ( mmap->type == MULTIBOOT_MEMORY_AVAILABLE){
        
                memInfo->TotalMemory +=  mmap->len;   
            } else {
                memInfo->ReservedMemory += mmap->len;
            }
            
            print_Multiboot_memory_Map(mmap);
            
        }
    
}


/**
 * @brief Debug Verbose functions
 * 
 * @param mmap 
 */

void print_Multiboot_memory_Map(multiboot_memory_map_t* mmap) {
    printf(
        "size = 0x%x, base_addr = 0x%x%08x, length = 0x%x%08x, type = 0x%x\n",
        (unsigned) mmap->size,
        (unsigned) (mmap->addr >> 32),
        (unsigned) (mmap->addr & 0xffffffff),
        (unsigned) (mmap->len >> 32),
        (unsigned) (mmap->len & 0xffffffff),
        (unsigned) mmap->type
    );
}

