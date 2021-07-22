#include "kernel.h"

extern "C" {

     multiboot_info_t *mbi;
    void early_main(unsigned long magic, unsigned long addr){

        /** initialize terminal interface */ 
        kterm_init();
      
       

        if (magic != MULTIBOOT_BOOTLOADER_MAGIC){
            printf("Invalid magic number: 0x%x\n", (unsigned) magic);
            return;
        }

        /* Set MBI to the addresss of the multiboot information structure*/
        mbi = (multiboot_info_t *) addr;

        /* Print out the flags */
        printf("flags = 0x%x\n", (unsigned) mbi->flags);

        /* Are mem_* valid? */
        if ( CHECK_FLAG(mbi->flags,0)){
            printf("mem_lower = %uKB, mem_upper = %uKB\n");
        }

        /* is boot device valid ? */
        if (CHECK_FLAG (mbi->flags, 1)){
            printf("boot_device = 0x0%x\n", (unsigned) mbi->boot_device);
        }

        /* is the command line passed? */
        if (CHECK_FLAG ( mbi->flags,2)){
            printf("cmdline = %s\n", (char *) mbi->cmdline);
        }

        /* Are mods_* valid? */
        if(CHECK_FLAG ( mbi->flags, 3)){
            multiboot_module_t *mod;
            int i;

            printf("mods count = %d, mods_addr = 0x%x\n", (int) mbi->mods_count, (int) mbi->mods_addr);

            for(i = 0, mod = (multiboot_module_t *) mbi->mods_addr; i < mbi->mods_count; i++ , mod++){
                printf(" mod start = 0x%x, mod_end = 0x%x, cmdline = %s\n", (unsigned) mod->mod_start, (unsigned) mod->mod_end, (char*) mod->cmdline);
            }
        }

        /* Bits 4 and 5 are mutually exclusive! */
        if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5)){
            printf("Both bits 4 and 5 are set.\n");
            return;
        }

        /* Is the symbol table of a.out valid? */
        if (CHECK_FLAG(mbi->flags, 4)){
            multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);

            printf( "multiboot_aout_symbol_table: tabsize = 0x%0x, strsize = 0x%x, addr = 0x%x\n", 
            (unsigned) multiboot_aout_sym->tabsize,
            (unsigned) multiboot_aout_sym->strsize,
            (unsigned) multiboot_aout_sym->addr);

        }

        /* Is the section header table of ELF valid? */
        if (CHECK_FLAG(mbi->flags, 5)){
            multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

            printf("multiboot_elf_sec: num = %u, size = 0x%x, addr = 0x%x, shnd = 0x%x\n",
            (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
            (unsigned) multiboot_elf_sec->addr, (unsigned)  multiboot_elf_sec->shndx);
            
        }


        // AAAAAH memory map, Yes please!
        /* Are mmap_* valid? */
        if (CHECK_FLAG(mbi->flags, 6)){
            multiboot_memory_map_t *mmap;
            
            printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
            (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
            
            for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr; 
                (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length; 
                mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size))){
                    
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
