#include "kernel.h"
#include "arch/i386/gdt/gdtc.h"
extern "C" {

    void early_main(unsigned long magic, unsigned long addr){
        /** initialize terminal interface */ 
        kterm_init();
        
        if (magic != MULTIBOOT_BOOTLOADER_MAGIC){
            printf("Invalid magic number: 0x%x\n",  magic);
            return;
        }

        CheckMBT(  (multiboot_info_t *) addr);


        multiboot_info_t* mbt = (multiboot_info_t*) addr;

        // Map the kernel
        //initPhysicalMemoryManager();

        // AAAAAH memory map, Yes please!
        /* Are mmap_* valid? */
        if (CHECK_FLAG(mbt->flags, 6)){
            multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) mbt->mmap_addr;
            
            printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
            (unsigned) mbt->mmap_addr, (unsigned) mbt->mmap_length);
            
            for (mmap;  (unsigned long) mmap < mbt->mmap_addr + mbt->mmap_length;  mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size))){
                    
                    printf(
                        "size = 0x%x, base_addr = 0x%x%08x, length = 0x%x%08x, type = 0x%x\n",
                        (unsigned) mmap->size,
                        (unsigned) (mmap->addr >> 32),
                        (unsigned) (mmap->addr & 0xffffffff),
                        (unsigned) (mmap->len >> 32),
                        (unsigned) (mmap->len & 0xffffffff),
                        (unsigned) mmap->type);
                
                }

        }

        /* Draw diagonal blue line */
        if (CHECK_FLAG (mbi->flags, 12)){
            printf("Can draw!");
        } 

        int cpu_model = get_model();
        int local_apic = check_apic();
        printf( "CPU Model: %x, Local APIC: %D\n", cpu_model, local_apic);

    }

    void kernel_main (void) {

        init_serial();





        while (true){
            //Read time indefinetely 
            read_rtc();
            printf( "UTC time: %02d-%02d-%02d %02d:%02d:%02d  [ Formatted as YY-MM-DD h:mm:ss]\r" ,year, month, day, hour, minute, second);
            delay(1000);
        }


       
       
    }   
}
