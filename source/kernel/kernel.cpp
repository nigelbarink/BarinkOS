#include "kernel.h"
extern "C" void early_main()
{
    kterm_init();
    initGDT();


    init_serial();
    print_serial("Hello Higher half kernel!\n");
 
    init_idt();
    // Enable interrupts
    asm volatile("STI");

    /*
     * Show a little banner for cuteness
     */
    printf("|===    BarinkOS       ===|\n");
    printf("Kernel End Addr: 0x%x\n" , &kernel_end );

    uint32_t PageDirectoryEntryIndex = ((uint32_t)&kernel_end + KERNEL_BASE_ADDR ) >> 22 ;

    uint32_t PageTableEntryIndex = (((uint32_t)&kernel_end + KERNEL_BASE_ADDR) >> 12) & 0x1FFF;
    
    printf("Kernel End PDE: %d, PTE: %d\n" , PageDirectoryEntryIndex, PageTableEntryIndex);

    uint32_t BootInfoStruct = BootInfoBlock_pptr + 0xC0000000;
    printf("Addr BootInfostruct: 0x%x\n", BootInfoStruct);

    uint32_t PageDirectoryEntryIndex2 = (BootInfoStruct  ) >> 2 ;

    uint32_t PageTableEntryIndex2 = (BootInfoStruct >> 12) & 0x1FFF;

    printf("PDE: 0x%x, PTE: 0x%x\n",  PageDirectoryEntryIndex2 , PageTableEntryIndex2 );

    BootInfoBlock* BootInfo = (BootInfoBlock*) ( BootInfoBlock_pptr + 0xC0000000 );


    printf("Size of BootInfoBlock: %d bytes\n", sizeof(BootInfoBlock));
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
        printf("MemoryInfoheap size : %d bytes\n", BootInfo->map_size);
        // Print the memory regions 
        MemoryInfoBlock* currentBlock = (MemoryInfoBlock*) ((uint32_t)BootInfo->MemoryMap + KERNEL_BASE_ADDR) ;

        printf( "Starting address: 0x%x\n", currentBlock);
        while( (uint32_t)currentBlock->next != 0x0 )
        {
            printf("CurrentBlock: 0x%x \n", (uint32_t ) currentBlock );
            if(IS_AVAILABLE_MEM(currentBlock->type)){
                //printf("AVAILABLE RAM\n");
            }
            else if(IS_ACPI_MEM(currentBlock->type)){
                //printf("ACPI MEMORY\n");   
            }
            else if(IS_RESERVED_MEM(currentBlock->type)){
               // printf("RESERVED MEMORY\n");
            }
            else if(IS_NVS_MEMORY(currentBlock->type)){
               // printf("NVS MEMORY \n");
            }
            else if(IS_BADRAM_MEMORY(currentBlock->type)){
               // printf("BADRAM MEMORY \n");
            }
            else {
              //  printf("(TYPE 0x%x )TYPE NOT SPECIFIED\n", currentBlock->type);
            }

            currentBlock = (MemoryInfoBlock*) ((uint32_t)currentBlock->next + KERNEL_BASE_ADDR );
          
        }

        printf("Starting physical memory management setup\n");
        // Setup PhysicalMemoryManagement
        SetupPhysicalMemoryManager(BootInfo);
     
    }
    printf("Enable Protected mode and jump to kernel main\n");
	asm volatile("mov %cr0, %eax ");
    asm volatile("or $1, %eax");
    asm volatile("mov %eax, %cr0"); // re-enable protected mode ? 
    kernel_main();
    
}

void PhysicalMemoryAllocatorTest(){
    #ifdef UNIT_TESTS
        // Small test!
        void* block = allocate_block();
        void* block2 = allocate_block();
        printf("Allocated addresss 1: 0x%x 2: 0x%x\n", (uint32_t)block ,(uint32_t)block2);
        free_block(block);
        free_block(block2);
        void* block3 = allocate_block();
        printf("Allocated addresss 3: 0x%x\n", (uint32_t)block3);
        free_block(block3);
    #endif
}


extern "C" void kernel_main () {
    pit_initialise();

    startSuperVisorTerminal();
}   