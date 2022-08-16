#include "paging.h"
PageDirectoryEntry kernel_directory[MAX_DIRECTORY_ENTRIES]__attribute__((aligned(4096)));
PageTableEntry first_page_table[1024]__attribute__((aligned(4096)));




#define KERNEL_VRT_MEMORY_BEGIN 0xC0000000
#define KERNEL_VRT_MEMORY_END   0xCFFFFFFF
#define PAGE_SIZE  0xFA0;


void InitializePaging()
{


    printf("\nInit paging\n");
    // The basics as explained by wiki.osdev.org

    // Set all page_directories to not present
    int i = 0;
    while ( i < 1024)
    {
        kernel_directory[i] = 0x00000002;
        i++;
    }


    // map 4 megabytes 
    unsigned int j ;
    for( j = 0;  j < 1024; j++ )
    {
        first_page_table[j] = (j * 0x1000) | 3 ;
        /*
        Attributes: 
        Supervisor Level ,
        read/write,
        present,
        */ 

    }

    // Put the page table in the page directory 
    // attributes: supervisor level, read/write, present;
    kernel_directory[0] = ((unsigned int)first_page_table) | 3;

    printf("Init paging DONE\n");
    // NOTE: Adjust this as needed 
     
    // BIOS Address Identity mapping 
    // Identity map the first 8MB ... Physical addresses 0x00000000 to 0x007A1200
    /*
    
    PHYSICAL_ADDRESS BIOSAddr = 0x00000000;
    PHYSICAL_ADDRESS BIOSAddr_Max = 0x007A1200;

    do
    {
        Map( BIOSAddr, BIOSAddr, *kernel_directory);
        BIOSAddr += PAGE_SIZE 
    } while(BIOSAddr <= BIOSAddr_Max);

    // Identity map the kernel space 
    VIRTUAL_ADDRESS Vaddr = KERNEL_VRT_MEMORY_BEGIN;
    PHYSICAL_ADDRESS KernelAddr = kernel_begin;

    do
    {
        Map(KernelAddr, Vaddr , *kernel_directory);

        Vaddr += PAGE_SIZE;
        KernelAddr += PAGE_SIZE 

    }
    while(KernelAddr < kernel_end);


    //TODO: Identity map VGA memory
   */
    
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

void Unmap(VIRTUAL_ADDRESS vaddr, PageDirectoryEntry& page_directory)
{
    // NOTE: I will implement lazy unmapping for now 
    
}


void Enable()
{ 

    //TODO: Write  protect will not be turned on 
    // for the moment altough according to the intel
    // developer manual this should happen.

    uint32_t CR0;
    
    CR0 =  GetCR0();
    printf("PG bit = %d \n" , GET_PG_BIT(CR0));

    printf("Load into CR3 address: 0x%x\n", (uint32_t)(&kernel_directory[0]));
    loadPageDirectory(&kernel_directory[0]);
    enablePaging();

    printf("Paging enabled!\n");


    CR0 = GetCR0();
    printf("PG bit = %d \n" , GET_PG_BIT(CR0) );


    uint32_t EFLAGS = GetEFLAGS();


}
