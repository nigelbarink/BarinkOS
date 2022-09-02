#include "VirtualMemoryManager.h"

extern uint32_t boot_page_directory[1024] ;
extern uint32_t boot_page_table[1024];


void AllocatePage(uint32_t vaddr)
{
    uint32_t page_aligned_address = ALIGN(vaddr, 4096);

    // allocate a page at  virtual address
    int PageDirectoryEntryIndex = vaddr >> 22;
    int PageTableEntryIndex = (vaddr >> 12) & 0x1FFF;

    printf("Allocation happening at PDE: %d PTE: %d\n", PageDirectoryEntryIndex, PageTableEntryIndex);

    // check if the page directory entry is marked as present 
    if (boot_page_directory[PageDirectoryEntryIndex] & 0x1 ) {

        uint32_t* page_table = (uint32_t*)((boot_page_directory[PageDirectoryEntryIndex]) & 0xFFFFE000 + 0xC0000000);

        // check if the page table entry is marked as present 
        if ( page_table[PageTableEntryIndex] & 0x1 )
        {   
            // Map the entry to a physical page
            page_table[PageTableEntryIndex] = (uint32_t)(allocate_block() + 0x3);
        } else{
            // mark page as present
            page_table[PageTableEntryIndex] =   0x3;
        }

    } else {
        // mark the page table as present and allocate a physical block for it
        boot_page_directory[PageDirectoryEntryIndex] = (uint32_t)(allocate_block() + 0x3);
    }


}

void FreePage(uint32_t vaddr )
{
    uint32_t page_aligned_address = ALIGN(vaddr, 4096);

    // allocate a page at  virtual address
    int PageDirectoryEntryIndex = vaddr >> 22;
    int PageTableEntryIndex = (vaddr >> 12) & 0x1FFF;

    uint32_t* pageTable = (uint32_t*)(boot_page_directory[PageDirectoryEntryIndex] & 0xFFFFE000 + 0xC0000000);


    void* physicalAddressToFree = (void*)(pageTable[PageTableEntryIndex] & 0xFFFFE000 + 0xC0000000);
    free_block(physicalAddressToFree);

    pageTable[PageTableEntryIndex] = 0x0;


}

void Map (  uint32_t vaddr, uint32_t paddr)
{
    uint32_t page_aligned_address = ALIGN(vaddr, 4096);

    // allocate a page at  virtual address
    int PageDirectoryEntryIndex = vaddr >> 22;
    int PageTableEntryIndex = (vaddr >> 12) & 0x1FFF;

}

void Unmap(uint32_t vaddr)
{
    // NOTE: I will implement lazy unmapping for now 
    uint32_t page_aligned_address = ALIGN(vaddr, 4096);

    // allocate a page at  virtual address
    int PageDirectoryEntryIndex = vaddr >> 22;
    int PageTableEntryIndex = (vaddr >> 12) & 0x1FFF;
}
