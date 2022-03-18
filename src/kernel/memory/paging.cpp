#include "paging.h"
PageDirectoryEntry kernel_directory[MAX_DIRECTORY_ENTRIES];

void InitializePaging()
{
    // Identity map the first 8MB ... Physical addresses 0x00000000 to 0x007A1200


    // Identity map the kernel space || our map to 3GB space


    // Identity map VGA memory

    
}


void AllocatePage(VIRTUAL_ADDRESS vaddr, PageDirectoryEntry& page_directory)
{

}


void FreePage(VIRTUAL_ADDRESS vaddr , PageDirectoryEntry& page_directory)
{

}


void Map ( PHYSICAL_ADDRESS paddr, VIRTUAL_ADDRESS vaddr, PageDirectoryEntry& page_directory)
{

}

void Unmap(VIRTUAL_ADDRESS, PageDirectoryEntry& page_directory)
{
    // NOTE: I will implement lazy unmapping for now 
    
}


void Enable()
{
    printf("Load into CR3 address: 0x%x\n", (uint32_t)(&kernel_directory[0]));
    loadPageDirectory(&kernel_directory[0]);
    enablePaging();

    printf("Paging enabled!\n");
}
