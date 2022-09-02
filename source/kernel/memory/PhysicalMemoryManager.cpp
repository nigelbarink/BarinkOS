#include "./PhysicalMemoryManager.h"

const uint32_t KERNEL_OFFSET = 0xC0000000;
extern uint32_t* boot_page_directory;
extern uint32_t* boot_page_table;
extern uint32_t* multiboot_page_table;

PhysicalMemoryManagerInfoBlock* PMMInfoBlock;

void SetupPhysicalMemoryManager( BootInfoBlock* Bootinfo) 
{

    // NOTE: We should move our bitmap to just after the end of our kernel instead
    PMMInfoBlock = (PhysicalMemoryManagerInfoBlock*) ( ((uint32_t)MemoryMapHeap_pptr + 80 ) + KERNEL_OFFSET );
    /*
        Every byte contains 8 pages
        A page is 4096 kib 
        Every block (1 bit) represent an page 
    */

    // Calculate the maximum number of blocks
    int maximum_blocks = (uint32_t)Bootinfo->MemorySize / BLOCK_SIZE / 8;
    PMMInfoBlock->max_blocks = maximum_blocks;
    PMMInfoBlock->used_blocks = 0;
    
    // put the map after the gdt
    PMMInfoBlock->memoryBitMap = (uint32_t*) ( 0xC010b100) ;
    
    //Size of memory map 
    uint32_t memMap_size = PMMInfoBlock->max_blocks / 8;
    // Set all places in memory as free
    memset(PMMInfoBlock->memoryBitMap, 0xFF, memMap_size  ); 
    MemoryInfoBlock* currentBlock = (MemoryInfoBlock*) ((uint32_t)Bootinfo->MemoryMap + 0xC0000000) ;

    printf( "Starting address: 0x%x\n", currentBlock);
    while( (uint32_t)currentBlock->next != 0x0 )
    {
        
        if(IS_AVAILABLE_MEM(currentBlock->type)){
           printf("skip!\n");
        }
        else{
            printf("allocate region 0x%x of size %d bytes\n", currentBlock->Base_addr, currentBlock->Memory_Size);
            // allocate_region( currentBlock->Base_addr, currentBlock->Memory_Size); // allocate region causes #PF Exception
        }

        currentBlock = (MemoryInfoBlock*) ((uint32_t)currentBlock->next + 0xC0000000 );
        
    }

    uint32_t kernel_size = ((uint32_t)&kernel_end - (uint32_t)&kernel_begin ) - KERNEL_OFFSET;

    printf("kernel size in memory: 0x%x\n", kernel_size);
    allocate_region((uint32_t)&kernel_begin, kernel_size);

    printf("allocate BIOS region\n");
    allocate_region (0x0000000, 0x00100000);
}

// NOTE: This can only give blocks of 4kb at a time!
//         We might at some point want to allocate multiple blocks at once.
void* allocate_block() {
    uint8_t blocks_available = PMMInfoBlock->max_blocks - PMMInfoBlock->used_blocks;
    // Are there any blocks available?
    if( blocks_available <= 0)
    {
        printf("No blocks available. Blocks Delta: 0x%x\n", blocks_available);
        return 0;
    }

    // Find 1 free block somewhere
    int free_block_index = bitmap_first_unset(PMMInfoBlock->memoryBitMap, PMMInfoBlock->max_blocks / 8 );

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
    

    for( int i = 0; i < NumberOfBlocksToAllocate; i++)
    {
        bitmap_unset(PMMInfoBlock->memoryBitMap, startBlock+ i);
        PMMInfoBlock->used_blocks++;
    }


}

void deallocate_region(uint32_t  StartAddress , uint32_t size ) {
    // reverse of what happened in allocate_region

    int NumberOfBlocks = (size / 1024) / 4 / 8 + 1;
    int startBlock = (StartAddress / 1024) / 4 / 8;

    for(int i = 0; i < NumberOfBlocks; i++)
    {
        bitmap_set(PMMInfoBlock->memoryBitMap, startBlock + i);
        PMMInfoBlock->used_blocks --;
    }
}


