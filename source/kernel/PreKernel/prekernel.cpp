#include <stdint.h>
#include <stddef.h>
#include "multiboot.h"
#include "bootstructure.h"
#define CHECK_FLAG(flags, bit) ((flags) & (1 <<(bit)))


extern "C" void testLauncher  ( unsigned long magic, multiboot_info_t* mbi) {
  
  // Create the bootInfoBlock at its location
  BootInfoBlock* BIB = (BootInfoBlock*) BootInfoBlock_pptr;

  /*
   * Check Multiboot magic number
   */ 
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
  {
        BIB->MapIsInvalid = true;
        return;
  } else{
        BIB->MapIsInvalid = false;
  }

  /* is boot device valid ? */
  if (CHECK_FLAG (mbi->flags, 1))
  {
        BIB->bootDeviceID = mbi->boot_device;
  }

  /* Are mods_* valid? */
  if(CHECK_FLAG ( mbi->flags, 3)){
      multiboot_module_t *mod;
      uint32_t i;

      BIB->GrubModuleCount = mbi->mods_count;


      for(i = 0, mod = (multiboot_module_t *) mbi->mods_addr; i < mbi->mods_count; i++ , mod++){
       
      }
  }

  /* Is the symbol table of a.out valid? */
  if (CHECK_FLAG(mbi->flags, 4))
  {
          BIB->ValidSymbolTable = true;
          multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);

  } else{
          BIB->ValidSymbolTable = false;
  }

  /* Is the section header table of ELF valid? */
  if (CHECK_FLAG(mbi->flags, 5))
  {
          BIB->ValidELFHeader = true;
          multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

  }else{
          BIB->ValidELFHeader = false;
  }
/*
        If we got a memory map from our bootloader we 
        should be parsing it to find out the memory regions available.
    */
    if (CHECK_FLAG(mbi->flags, 6))
    {  
        BIB->PhysicalMemoryMapAvailable = true;
        multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) (mbi->mmap_addr) ;

        for (;  (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;  mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size))){

            if ( mmap->type == MULTIBOOT_MEMORY_AVAILABLE){
                  
            } else{
               
               
            }
          
            
        } 
        
    } else{
        BIB->PhysicalMemoryMapAvailable = false;
    }
    
  /* Draw diagonal blue line */
  if (CHECK_FLAG (mbi->flags, 12)){
          BIB->EnabledVBE = true;
  } else{
          BIB->EnabledVBE;
  }
}
