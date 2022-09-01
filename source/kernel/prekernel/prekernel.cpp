#include <stdint.h>
#include <stddef.h>
#include "multiboot.h"
#include "bootstructure.h"
#define CHECK_FLAG(flags, bit) ((flags) & (1 <<(bit)))


extern "C" void prekernelSetup  ( unsigned long magic, multiboot_info_t* mbi) {
  
  // Create the bootInfoBlock at its location
  BootInfoBlock* BIB = (BootInfoBlock*) BootInfoBlock_pptr;

  /*
   * Check Multiboot magic number
   */ 
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
  {
        BIB->MapIsInvalid = true;
        // crash
        return;
  } else{
        BIB->MapIsInvalid = false;
  }

  /* is boot device valid ? */
  if (CHECK_FLAG (mbi->flags, 1))
  {
        BIB->bootDeviceID = mbi->boot_device;
  } else{
        BIB->bootDeviceID = 0x11111111;
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
        BIB->MemoryMap = (MemoryInfoBlock*) MemoryMapHeap_pptr;
        multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) (mbi->mmap_addr) ;
        auto MemoryMapEnd =  mbi->mmap_addr + mbi->mmap_length;
        
        auto CurrentInfoBlock = BIB->MemoryMap;
      
        uint32_t RAM_size = 0;

        while((unsigned long) mmap < MemoryMapEnd){
                BIB->map_size += sizeof(MemoryInfoBlock);
                CurrentInfoBlock->Base_addr = mmap->addr;
                CurrentInfoBlock->Memory_Size = mmap->len;
                

                if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
                        CurrentInfoBlock->type |= 0x1;
                        RAM_size += mmap->len;
                if(mmap->type == MULTIBOOT_MEMORY_ACPI_RECLAIMABLE)
                        CurrentInfoBlock->type |= 0x2;
                if(mmap->type == MULTIBOOT_MEMORY_RESERVED)
                        CurrentInfoBlock->type |= 0x4;
                if(mmap->type == MULTIBOOT_MEMORY_NVS)
                        CurrentInfoBlock->type |= 0x8;
                if(mmap->type == MULTIBOOT_MEMORY_BADRAM)
                        CurrentInfoBlock->type |= 0x10;
                

                // continue to the next block
                mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof(mmap->size));
              
              CurrentInfoBlock->next = (MemoryInfoBlock*) ((uint32_t)CurrentInfoBlock) + sizeof(MemoryInfoBlock);
              CurrentInfoBlock = CurrentInfoBlock->next;

        }

        CurrentInfoBlock->next = (MemoryInfoBlock*) 0x0;
        BIB->MemorySize = RAM_size;
} else
{
        BIB->PhysicalMemoryMapAvailable = false;
}
    
  /* Draw diagonal blue line */
  if (CHECK_FLAG (mbi->flags, 12)){
          BIB->EnabledVBE = true;
  } else{
          BIB->EnabledVBE;
  }
}
