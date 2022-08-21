#include "paging.h"
// PageDirectoryEntry kernel_directory[MAX_DIRECTORY_ENTRIES]__attribute__((aligned(4096)));
// PageTableEntry first_page_table[MAX_PAGE_TABLE_ENTRIES]__attribute__((aligned(4096)));

void IdentityMap (){
        printf("\nInit paging\n");
    // The basics as explained by wiki.osdev.org

    // Set all page_directories to not present
    int i = 0;
    while ( i < 1024)
    {
     //   kernel_directory[i] = 0x2;
        i++;
    }


    // map 4 megabytes 
    unsigned int j ;
    for( j = 0;  j < 1024; j++ )
    {
      //  first_page_table[j] = (j * 0x1000) | 3 ;
        
        //Attributes: 
        //Supervisor Level ,
        //read/write,
        //present,
        

    }

    // Put the page table in the page directory 
    // attributes: supervisor level, read/write, present;
   // kernel_directory[0] = ((unsigned int)first_page_table) | 3;

    printf("Init paging DONE\n");
}

void InitializePaging()
{
    /*
        Initial kernel page directory
         set all page tables to not present
    */
   
    for (int i = 0; i < MAX_DIRECTORY_ENTRIES; i++)
    {
    //    kernel_directory[i] = 0x2;
    }

    // BIOS Address Identity mapping 
    // Identity map the first 8MiB ... Physical addresses 0x00000000 to 0x007A1200
    PHYSICAL_ADDRESS BIOSAddr = 0x00000000;
    PHYSICAL_ADDRESS BIOSAddr_Max = 0x800000;

    // How many PDE's do we need
    uint8_t NUM_PDE = BIOSAddr_Max / (4 * 1024 * 1024);

    printf("The first 8MiB require %d Page Directory Entries\n", NUM_PDE);
    

    for( int i = 0; i < NUM_PDE; i++)
    {
        // setup a page table 
       // PageTableEntry pagetable[MAX_PAGE_TABLE_ENTRIES] = PhysicalMemory::allocate_block(); // TODO :: Physical memory manager functions should be available here.

        for(int j = 0; j < MAX_PAGE_TABLE_ENTRIES; j++)
        {
       //     pagetable[j] = ( j  *  4096 ) | 3;
        }

        // add page table as page directory entry 
     //   kernel_directory[i] =  ( (unsigned int) pagetable ) | 3;
    }

    //  map the kernel space 
    VIRTUAL_ADDRESS Vaddr = KERNEL_VRT_MEMORY_BEGIN;
    PHYSICAL_ADDRESS KernelAddr = kernel_begin;
    PHYSICAL_ADDRESS KernelEndAddr = kernel_end;

    uint32_t KernelSizeInBytes = (KernelEndAddr - KernelAddr);
    printf("Kernel is 0x%x bytes\n", KernelSizeInBytes);
    NUM_PDE = KernelSizeInBytes / (4 * 1024* 1024);
    printf("Kernel requires %d Page Directory Entries\n", NUM_PDE);


    for(int i = 0; i < NUM_PDE; i++)
    {
     //   PageTableEntry pageTable [MAX_PAGE_TABLE_ENTRIES] = PhysicalMemory::allocate_block();

        for(int j = 0; j < MAX_PAGE_TABLE_ENTRIES; j++)
        {
        //    pageTable[j] = ( j * 4096) | 3; // NOTE: Check if page is actually supposed to be present
        }

        // TODO: Calculate Page Directory index 
        


    }
    

    


    // Identity map VGA memory
    // Calc which PDE adn 
    
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

   // printf("Load into CR3 address: 0x%x\n", (uint32_t)(&kernel_directory[0]));
   // loadPageDirectory(&kernel_directory[0]);
   // enablePaging();

    printf("Paging enabled!\n");


    CR0 = GetCR0();
    uint32_t CR4 = GetCR4();
    printf("PG bit = %d\n" , GET_PG_BIT(CR0) );
    printf("PAE bit = %d\n", GET_PAE_BIT(CR4));

    if(GET_PAE_BIT(CR4) == 0){
        printf("Using 32bit paging\n");

        if(GET_PSE_BIT(CR4) == 0 ){
            printf("Page size is 4KiB\n");
        } else { 
            printf("Page size is 4MiB\n");
        }
    }  else {
        printf("Using some extended version for paging\n");
    }

}
