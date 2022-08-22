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