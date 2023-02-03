#pragma once 
#include "boot/multiboot.h"
#define CHECK_FLAG(flags, bit) ((flags) & (1 <<(bit)))

#include "tty/kterm.h"



void CheckMBT ( multiboot_info_t*  mbt  ){
      /* Set MBI to the addresss of the multiboot information structure*/
        multiboot_info_t * mbi = (multiboot_info_t *) mbt;

#ifdef __VERBOSE__
        /* Print out the flags */
        printf("flags = 0x%x\n", (unsigned) mbi->flags);
#endif
        /* Are mem_* valid? */
        if ( CHECK_FLAG(mbi->flags,0)){
                // Do nothing
        }

        /* is boot device valid ? */
        if (CHECK_FLAG (mbi->flags, 1))
        {
#ifdef __VERBOSE__
            printf("boot_device = 0x0%x\n", (unsigned) mbi->boot_device);
#endif
        }

        /* is the command line passed? */
        if (CHECK_FLAG ( mbi->flags,2))
        {
#ifdef  __VERBOSE__
            printf("cmdline = %s\n", (char *) mbi->cmdline);
#endif
        }

        /* Are mods_* valid? */
        if(CHECK_FLAG ( mbi->flags, 3)){
            multiboot_module_t *mod;
            uint32_t i;
#ifdef __VERBOSE__
            printf("mods count = %d, mods_addr = 0x%x\n", (int) mbi->mods_count, (int) mbi->mods_addr);

            for(i = 0, mod = (multiboot_module_t *) mbi->mods_addr; i < mbi->mods_count; i++ , mod++){
                printf(" mod start = 0x%x, mod_end = 0x%x, cmdline = %s\n", (unsigned) mod->mod_start, (unsigned) mod->mod_end, (char*) mod->cmdline);
            }
#endif
        }

        /* Bits 4 and 5 are mutually exclusive! */
        if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG(mbi->flags, 5))
        {
#ifdef __VERBOSE__
            printf("Both bits 4 and 5 are set.\n");
#endif 
            return;
        }

        /* Is the symbol table of a.out valid? */
        if (CHECK_FLAG(mbi->flags, 4)){
            multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);
#ifdef __VERBOSE__
            printf( "multiboot_aout_symbol_table: tabsize = 0x%0x, strsize = 0x%x, addr = 0x%x\n", 
            (unsigned) multiboot_aout_sym->tabsize,
            (unsigned) multiboot_aout_sym->strsize,
            (unsigned) multiboot_aout_sym->addr);
#endif
        }

        /* Is the section header table of ELF valid? */
        if (CHECK_FLAG(mbi->flags, 5)){
            multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);
#ifdef __VERBOSE__
            printf("multiboot_elf_sec: num = %u, size = 0x%x, addr = 0x%x, shnd = 0x%x\n",
            
            (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
            (unsigned) multiboot_elf_sec->addr, (unsigned)  multiboot_elf_sec->shndx);
#endif
            
        }

        /* Draw diagonal blue line */
        if (CHECK_FLAG (mbt->flags, 12)){
#ifdef __VERBOSE__
            printf("Can draw!\n");
#endif
        } 


}