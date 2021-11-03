#include "PageFrameAllocator.h"


MemoryInfo memInfo {};
void mapMultibootMemoryMap( multiboot_info_t *mbt){
    printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
    (unsigned) mbt->mmap_addr, (unsigned) mbt->mmap_length);
    multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) mbt->mmap_addr;

    for (;  (unsigned long) mmap < mbt->mmap_addr + mbt->mmap_length;  mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size))){

            if ( mmap->type == MULTIBOOT_MEMORY_AVAILABLE){
                memInfo.memorySizeInBytes +=  mmap->len;   
            } else {
                memInfo.reservedMemoryInBytes += mmap->len;
            }
            
            print_Multiboot_memory_Map(mmap);
            
        }
    uint32_t memorySizeInGiB = memInfo.memorySizeInBytes / 1073741824; 

    printf("Available Memory: 0x%x bytes, 0x%x GiB\n",  memInfo.memorySizeInBytes, memorySizeInGiB );
    printf("Reserved Memory: 0x%x bytes\n",  memInfo.reservedMemoryInBytes );

}

void print_Multiboot_memory_Map(multiboot_memory_map_t* mmap){
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