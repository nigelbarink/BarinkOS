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
            uint32_t memorySizeInBytes = 0;
            uint32_t reservedMemoryInBytes = 0;


            printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
            (unsigned) mbt->mmap_addr, (unsigned) mbt->mmap_length);
            
            for (;  (unsigned long) mmap < mbt->mmap_addr + mbt->mmap_length;  mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size))){
                    if ( mmap->type == MULTIBOOT_MEMORY_AVAILABLE){
                        memorySizeInBytes +=  mmap->len;   
                    } else {
                        reservedMemoryInBytes += mmap->len;
                    }


                    printf(
                        "size = 0x%x, base_addr = 0x%x%08x, length = 0x%x%08x, type = 0x%x\n",
                        (unsigned) mmap->size,
                        (unsigned) (mmap->addr >> 32),
                        (unsigned) (mmap->addr & 0xffffffff),
                        (unsigned) (mmap->len >> 32),
                        (unsigned) (mmap->len & 0xffffffff),
                        (unsigned) mmap->type);
                
                }
            uint32_t memorySizeInGiB = memorySizeInBytes / 1073741824; 
            
            printf("Available Memory: 0x%x bytes, 0x%x GiB\n",  memorySizeInBytes, memorySizeInGiB );
            printf("Reserved Memory: 0x%x bytes\n",  reservedMemoryInBytes );
        }
        
       

       //int cpu_model = get_model();
       //int local_apic = check_apic();
       //printf( "CPU Model: %x, Local APIC: %D\n", cpu_model, local_apic);


     /* Setup Paging and memory Managment*/
        //MMU  MemoryManagementUnit = MMU();
        //MemoryManagementUnit.enable(); // Warning: Causes triple page fault
        //printf("Pages available: %9d\n", pmm_available());

        /* Draw diagonal blue line */
        if (CHECK_FLAG (mbt->flags, 12)){
            printf("Can draw!");
        } 

        //setupGdt();

       
    }

    void kernel_main (void) {

        init_serial();

        while (false){
            //Read time indefinetely 
            read_rtc();
            printf( "UTC time: %02d-%02d-%02d %02d:%02d:%02d  [ Formatted as YY-MM-DD h:mm:ss]\r" ,year, month, day, hour, minute, second);
            delay(1000);
        }
       
    }   
}
