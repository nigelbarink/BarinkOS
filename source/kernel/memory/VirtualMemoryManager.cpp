#include "VirtualMemoryManager.h"
#define ALIGN(addr, align) (((addr) & ~((align) - 1 )) + (align))

extern uint32_t boot_page_directory[1024] ;
extern uint32_t boot_page_table[1024];


void flush_cr3(){
    asm volatile("movl %cr3, %ecx;"
	             "movl %ecx, %cr3");
}


void AllocatePage(uint32_t vaddr)
{
    uint32_t page_aligned_address = ALIGN(vaddr, 4096);

    // allocate a page at  virtual address
    int PageDirectoryEntryIndex = vaddr >> 22;
    int PageTableEntryIndex = (vaddr >> 12) & 0x1FFF;

    printf("Allocation happening at PDE: %d PTE: %d\n", PageDirectoryEntryIndex, PageTableEntryIndex);

    // check if the page directory entry is marked as present 
    if (boot_page_directory[PageDirectoryEntryIndex] & 0x1 ) 
    {
        printf("Directory entry is marked as present\n");
        uint32_t* page_table = (uint32_t*)((boot_page_directory[PageDirectoryEntryIndex]) & 0xFFFFE000) ;
        page_table = (uint32_t*) ((uint32_t)page_table + 0xC0000000); // Add kernel offset
        printf("Page table address: 0x%x\n", (uint32_t)page_table);

        // check if the page table entry is marked as present 
        if ( page_table[PageTableEntryIndex] & 0x1 )
        {   
            printf("page already present!\n");
            return;
        } else{
            printf("Mapping a physical page.\n");
            // Map the entry to a physical page
            page_table[PageTableEntryIndex] = (uint32_t)(allocate_block() + 0x3);
            flush_cr3();
        }

    } else {
        printf("Mapping a new page directory entry with a page table\n");
        // mark the page table as present and allocate a physical block for it
        boot_page_directory[PageDirectoryEntryIndex] = (uint32_t)(allocate_block() + 0x3);
        flush_cr3();
   
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


void Immediate_Map (  uint32_t vaddr, uint32_t paddr)
{
    uint32_t page_aligned_address = ALIGN(vaddr, 4096);

    // allocate a page at  virtual address
    int PageDirectoryEntryIndex = vaddr >> 22;
    int PageTableEntryIndex = (vaddr >> 12) & 0x1FFF;

}

void Immediate_Unmap(uint32_t vaddr)
{
    // NOTE: I will implement lazy unmapping for now 
    uint32_t page_aligned_address = ALIGN(vaddr, 4096);

    // allocate a page at  virtual address
    int PageDirectoryEntryIndex = vaddr >> 22;
    int PageTableEntryIndex = (vaddr >> 12) & 0x1FFF;
}
