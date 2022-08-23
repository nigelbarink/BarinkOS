#include "kernel.h"
extern "C" void early_main()
{
    /*
     * Initialize terminal interface 
     */ 
    initGDT();

    kterm_init();

    init_serial();
    print_serial("Hello Higher half kernel!\n");
 
    init_idt();
    // Enable interrupts
    asm volatile("STI");

    /*
     * Show a little banner for cuteness
     */
    printf("|===    BarinkOS       ===|\n");
    printf("Kernel End Addr: 0x%x\n" , &kernel_end + KERNEL_BASE_ADDR);

    uint32_t PageDirectoryEntryIndex = ((uint32_t)&kernel_end + KERNEL_BASE_ADDR ) >> 2 ;

    uint32_t PageTableEntryIndex = (((uint32_t)&kernel_end + KERNEL_BASE_ADDR) >> 12) & 0x1FFF;
    
    printf("Kernel End PDE: %d, PTE: %d\n" , PageDirectoryEntryIndex, PageTableEntryIndex);

    uint32_t BootInfoStruct = BootInfoBlock_pptr + 0xC0000000;
    printf("Addr BootInfostruct: 0x%x\n", BootInfoStruct);

    uint32_t PageDirectoryEntryIndex2 = (BootInfoStruct  ) >> 2 ;

    uint32_t PageTableEntryIndex2 = (BootInfoStruct >> 12) & 0x1FFF;

    printf("PDE: 0x%x, PTE: 0x%x\n",  PageDirectoryEntryIndex2 , PageTableEntryIndex2 );

    BootInfoBlock* BootInfo = (BootInfoBlock*) ( BootInfoBlock_pptr + 0xC0000000 );

    printf("Bootloader information:\n");
    if( BootInfo->ValidELFHeader )
    {
        printf("- Valid ELF Header is available!\n");
    }

    if(BootInfo->EnabledVBE)
    {
        printf("- VBE graphics mode is available!\n");
    }

    if(BootInfo->ValidSymbolTable)
    {
        printf("- Valid Symbol Table available at 0x%x.\n Tab Size: %d, str Size: %d\n", BootInfo->SymbolTableAddr, BootInfo->SymbolTabSize, BootInfo->SymbolStrSize);
    }

    if(BootInfo->PhysicalMemoryMapAvailable)
    {
        printf("- Physical Memory Map available!\n");

        // Print the memory regions 
        MemoryInfoBlock* currentBlock = (MemoryInfoBlock*) ((uint32_t)BootInfo->MemoryMap + KERNEL_BASE_ADDR) ;
  
        kterm_setcolor(VGA_COLOR_RED);
        printf("size of MemoryInfoBlock: 0x%x\n", sizeof(MemoryInfoBlock));
        kterm_setcolor(VGA_COLOR_CYAN);
        printf("Kernel End is at address: 0x%x\n", &kernel_end);
        printf("BootInfo is at address: 0x%x\n", BootInfo);
        printf("map is at address: 0x%x\n", currentBlock + KERNEL_BASE_ADDR);
        kterm_setcolor(VGA_COLOR_WHITE);
        while( (uint32_t)currentBlock->next != 0x0 )
        {
            kterm_setcolor(VGA_COLOR_CYAN);
            printf("map is at address: 0x%x\n", ( (uint32_t)currentBlock  ));
            kterm_setcolor(VGA_COLOR_WHITE);
      
           /* 
            uint32_t pageDirectoryIndex = ((uint32_t)&currentBlock )  >> 22;
            printf("pageDirectoryIndex: %d\n", pageDirectoryIndex);

            uint32_t pageTableIndex = ((uint32_t)&currentBlock  >> 12)  & 0x1FFF;
            printf("PagTableIndex: %d\n", pageTableIndex);
            */

            //printf("boot_page_directory addr: 0x%x\n", &boot_page_directory);
            //printf("boot_page_table addr: 0x%x\n", &multiboot_page_table);
            printf("Memory Region: \n");

            if(IS_AVAILABLE_MEM(currentBlock->type)){
                //printf("AVAILABLE RAM\n");
            }
            else if(IS_ACPI_MEM(currentBlock->type)){
                printf("ACPI MEMORY\n");   
            }
            else if(IS_RESERVED_MEM(currentBlock->type)){
                printf("RESERVED MEMORY\n");
            }
            else if(IS_NVS_MEMORY(currentBlock->type)){
                printf("NVS MEMORY \n");
            }
            else if(IS_BADRAM_MEMORY(currentBlock->type)){
                printf("BADRAM MEMORY \n");
            }
            else {
              //  printf("(TYPE 0x%x )TYPE NOT SPECIFIED\n", currentBlock->type);
            }
            
           // printf("Base address: 0x%x, Memory size: 0x%x\n", currentBlock->Base_addr, currentBlock->Memory_Size);

            currentBlock = (MemoryInfoBlock*) ((uint32_t)currentBlock->next + KERNEL_BASE_ADDR );
          
        }
        


    }
    
	asm volatile("mov %cr0, %eax ");
    asm volatile("or $1, %eax");
    asm volatile("mov %eax, %cr0");
    kernel_main();
    
}

void map_multiboot_info_structure(unsigned long addr){
    // map the multiboot structure into virtual memory 
    // so we can gather the necessary data from it.

    uint32_t pageDirectoryIndex = (addr )  >> 22;
    printf("pageDirectoryIndex: %d\n", pageDirectoryIndex);

    uint32_t pageTableIndex = (addr  >> 12)  & 0x1FFF;
    printf("PagTableIndex: %d\n", pageTableIndex);


    printf("boot_page_directory addr: 0x%x\n", &boot_page_directory);
    printf("boot_page_table addr: 0x%x\n", &multiboot_page_table);

    uint32_t* current_page_directory = &boot_page_directory;
    uint32_t* needed_page_table = &multiboot_page_table - KERNEL_BASE_ADDR;
    // set the page tabel reference;
    current_page_directory[pageDirectoryIndex]  = (uint32_t)&multiboot_page_table - KERNEL_BASE_ADDR + 0x003;

    // set the page reference;
    needed_page_table[ pageTableIndex ] = addr | 0x003;


    // Reload CR3 to force a flush
    asm("movl %cr3, %ecx;" "movl %ecx, %cr3" );
}

void PhysicalMemoryAllocatorTest(){
#ifdef UNIT_TESTS
       // test alloc_block
        uint8_t* memory = (uint8_t*) memAlloc.allocate_block();
        printf("Got a new pointer: 0x%x\n", memory);

        uint8_t* memory2 = (uint8_t*) memAlloc.allocate_block();
        printf("Got a new pointer: 0x%x\n", memory2);

        memAlloc.free_block((void*) memory);
        uint8_t* newBlockPlse = (uint8_t*) memAlloc.allocate_block();
#endif
}


extern "C" void kernel_main () {
    pit_initialise();

    // Create a dummy BootInfo object
    // TODO: This should be done properly or the dependency should
    //          be removed from the SuperVisorTerminal.
    BootInfo* bootinfo = {};

    startSuperVisorTerminal(bootinfo);
}   