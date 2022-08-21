#include "kernel.h"

void map_multiboot_info_structure(unsigned long addr);
extern "C" void kernel_main (BootInfo* bootinfo);
extern "C" uint32_t boot_page_directory;
extern "C" uint32_t multiboot_page_table;

const uint32_t KERNEL_BASE_ADDR = 0xC0000000;
extern "C" void early_main(unsigned long magic, unsigned long addr){

    // Convert MBI address to higher quarter kernel space
    addr += KERNEL_BASE_ADDR;
    /** 
     * Initialize terminal interface 
     * NOTE: This should be done later on , the magic value should be checked first.
     */ 
    initGDT();
    kterm_init();
    init_serial();
    print_serial("Hello Higher half kernel!\n");
 
    init_idt();
    // Enable interrupts
    asm volatile("STI");

   
    printf("DEBUG:\n Magic: 0x%x\n MBT_addr: 0x%x\n", magic, addr);
    /**
     * Check Multiboot magic number
     * NOTE: Printf call should not be a thing this early on ...
    */ 
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC){
        printf("Invalid magic number: 0x%x\n",  magic);
        return;
    }

    /**
     * Show a little banner for cuteness
     */
    printf("|===    BarinkOS       ===|\n");
    /**
     * Use the address given as an argument as the pointer
     * to a Multiboot information structure.
     */
        multiboot_info_t* mbt = (multiboot_info_t*) (addr );

    /**
     * Construct our own bootInfo structure
     */
        BootInfo bootinfo = {};
       
        
    /*
        If we got a memory map from our bootloader we 
        should be parsing it to find out the memory regions available.
    */
    if (CHECK_FLAG(mbt->flags, 6))
    {  
    
        /*
            Setup Physical memory managment
        */
        MemoryInfo meminfo = {};
        bootinfo.memory = &meminfo;
 
        mapMultibootMemoryMap(bootinfo.memory , mbt);        
        printf("Memory size: 0x%x bytes\n", bootinfo.memory->TotalMemory );

       /*
        PhysicalMemory memAlloc = PhysicalMemory{};
        memAlloc.setup(bootinfo.memory );
        */

        // TODO: FIX physical allocator
        /*
         Mark already in use sections 
        */
        
        // Mark kernel memory as used
        printf("Kernel Begin Pointer: 0x%x, Kernel end pointer: 0x%x\n", &kernel_begin , &kernel_end );
       
       
        multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) (mbt->mmap_addr + KERNEL_BASE_ADDR) ;

        for (;  (unsigned long) mmap < mbt->mmap_addr + mbt->mmap_length;  mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size))){

            if ( mmap->type == MULTIBOOT_MEMORY_AVAILABLE){
                  
            } else{
                printf("allocate region: 0x%x, size : 0x%x bytes\n", (unsigned) mmap->addr,(unsigned) mmap->len  );
               // memAlloc.allocate_region((unsigned)mmap->addr , (unsigned)mmap->len);
            }
          
            
        } 


       printf("allocate region: 0x%x, size : 0x%x bytes\n", &kernel_begin, &kernel_end - &kernel_begin );
       //memAlloc.allocate_region(kernel_end, kernel_end - kernel_begin);
    
        // test alloc_block
        /*
        uint8_t* memory = (uint8_t*) memAlloc.allocate_block();
        printf("Got a new pointer: 0x%x\n", memory);

        uint8_t* memory2 = (uint8_t*) memAlloc.allocate_block();
        printf("Got a new pointer: 0x%x\n", memory2);


        memAlloc.free_block((void*) memory);

        uint8_t* newBlockPlse = (uint8_t*) memAlloc.allocate_block();
        */



        //memAlloc.free_block((void*) memory);
        //InitializePaging();
        //IdentityMap();
        //Enable();
    } else{
        printf("memory flag not set!");
    }
    
    CheckMBT( (multiboot_info_t *) addr);
	asm volatile("mov %cr0, %eax ");
    asm volatile("or $1, %eax");
    asm volatile("mov %eax, %cr0");
    kernel_main(&bootinfo);
    
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

    
extern "C" void kernel_main (BootInfo* bootinfo) {
    pit_initialise();

    startSuperVisorTerminal(bootinfo);
}   