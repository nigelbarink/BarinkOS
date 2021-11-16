#include "PageDirectory.h"
#include <stdint.h>



void PageDirectory::enable(){
    // https://wiki.osdev.org/Setting_Up_Paging
    //set each entry to not present
    int i;
    for(i = 0; i < 1024; i++)
    {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //   Not Present: The page table is not present
        this->page_directory[i] = 0x00000002;
    }

    // holds the physical address where we want to start mapping these pages to.
    // in this case, we want to map these pages to the very beginning of memory.

    //we will fill all 1024 entries in the table, mapping 4 megabytes
    for(unsigned int i = 0; i < 1024; i++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
    }

    // attributes: supervisor level, read/write, present
    this->page_directory[0] = ((unsigned int)first_page_table) | 3;


    loadPageDirectory(this->page_directory);
    enablePaging();
}

/*
void IdentityPaging(uint32_t *first_pte, vaddr from, int size)
{
    from = from & 0xFFFFF000; // Discard the bits we don't want
    for (; size > 0; from += 4096, first_pte++)
    {
        *first_pte = from | 1; // makr page present.
    }
}
*/
