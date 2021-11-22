#pragma once 
#include "bootloader/multiboot.h"
#define CHECK_FLAG(flags, bit) ((flags) & (1 <<(bit)))

#include "tty/kterm.h"



void CheckMBT ( multiboot_info_t*  mbt  ){
      /* Set MBI to the addresss of the multiboot information structure*/
        multiboot_info_t * mbi = (multiboot_info_t *) mbt;

        /* Print out the flags */
        printf("flags = 0x%8x\n", (unsigned) mbi->flags);

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

        /* Draw diagonal blue line */
        if (CHECK_FLAG (mbt->flags, 11)){
            printf("Can draw!");
   
            multiboot_uint32_t color;
            unsigned i;
            void *fb = (void *) (unsigned long) mbt->framebuffer_addr;

      switch (mbt->framebuffer_type)
        {
        case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
          {
            unsigned best_distance, distance;
            struct multiboot_color *palette;
            
            palette = (struct multiboot_color *) mbt->framebuffer_palette_addr;

            color = 0;
            best_distance = 4*256*256;
            
            for (i = 0; i < mbi->framebuffer_palette_num_colors; i++)
              {
                distance = (0xff - palette[i].blue) * (0xff - palette[i].blue)
                  + palette[i].red * palette[i].red
                  + palette[i].green * palette[i].green;
                if (distance < best_distance)
                  {
                    color = i;
                    best_distance = distance;
                  }
              }
          }
          break;

        case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
          color = ((1 << mbt->framebuffer_blue_mask_size) - 1) 
            << mbt->framebuffer_blue_field_position;
          break;

        case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
          color = '\\' | 0x0100;
          break;

        default:
          color = 0xffffffff;
          break;
        }
      for (i = 0; i < mbt->framebuffer_width
             && i < mbt->framebuffer_height; i++)
        {
          switch (mbt->framebuffer_bpp)
            {
            case 8:
              {
                multiboot_uint8_t *pixel = fb + mbt->framebuffer_pitch * i + i;
                *pixel = color;
              }
              break;
            case 15:
            case 16:
              {
                multiboot_uint16_t *pixel
                  = fb + mbt->framebuffer_pitch * i + 2 * i;
                *pixel = color;
              }
              break;
            case 24:
              {
                multiboot_uint32_t *pixel
                  = fb + mbt->framebuffer_pitch * i + 3 * i;
                *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
              }
              break;

            case 32:
              {
                multiboot_uint32_t *pixel
                  = fb + mbt->framebuffer_pitch * i + 4 * i;
                *pixel = color;
              }
              break;
            }
        }

    }
}