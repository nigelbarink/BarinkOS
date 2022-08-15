#include "kernel.h"

extern "C" void kernel_main (BootInfo* bootinfo) {
    init_serial();
    pit_initialise();

    //InitializePaging();
    //Enable();

    startSuperVisorTerminal(bootinfo);
}   


extern "C" void early_main(unsigned long magic, unsigned long addr){
    /** 
     * Initialize terminal interface 
     * NOTE: This should be done later on , the magic value should be checked first.
     */ 
    kterm_init();
    
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
        multiboot_info_t* mbt = (multiboot_info_t*) addr;

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

        PhysicalMemory memAlloc = PhysicalMemory{};
        memAlloc.setup(bootinfo.memory );

        /*
         Mark already in use sections 
        */
        
        // Mark kernel memory as used
        printf("Kernel Begin Pointer: 0x%x, Kernel end pointer: 0x%x\n", kernel_begin , kernel_end );
       
        multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) mbt->mmap_addr;

        for (;  (unsigned long) mmap < mbt->mmap_addr + mbt->mmap_length;  mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size))){

            if ( mmap->type == MULTIBOOT_MEMORY_AVAILABLE){
                  
            } else{
                printf("allocate region: 0x%x, size : 0x%x bytes\n", (unsigned) mmap->addr,(unsigned) mmap->len  );
                memAlloc.allocate_region((unsigned)mmap->addr , (unsigned)mmap->len);
            }
          
            
        } 


       printf("allocate region: 0x%x, size : 0x%x bytes\n", kernel_begin, kernel_end - kernel_begin );
       memAlloc.allocate_region(kernel_end, kernel_end - kernel_begin);

        // test alloc_block
        /*
        uint8_t* memory = (uint8_t*) memAlloc.allocate_block();
        printf("Got a new pointer: 0x%x\n", memory);

        uint8_t* memory2 = (uint8_t*) memAlloc.allocate_block();
        printf("Got a new pointer: 0x%x\n", memory2);


        memAlloc.free_block((void*) memory);

        uint8_t* newBlockPlse = (uint8_t*) memAlloc.allocate_block();
        */



       // memAlloc.free_block((void*) memory);
        InitializePaging();
        Enable();
    }
    
    initGDT();
    init_idt();
    // Enable interrupts
    asm volatile("STI");
    

    CheckMBT(  (multiboot_info_t *) addr);


    kernel_main(&bootinfo);
    
}

