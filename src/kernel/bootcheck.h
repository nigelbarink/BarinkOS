#pragma once 
#include "bootloader/multiboot.h"
#define CHECK_FLAG(flags, bit) ((flags) & (1 <<(bit)))

extern "C" {
    #include "arch/i386/tty/kterm.h"
}



void CheckMBT ( multiboot_info_t*  mbt  ){
      /* Set MBI to the addresss of the multiboot information structure*/
        multiboot_info_t * mbi = (multiboot_info_t *) mbt;

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
            uint32_t i;

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


}