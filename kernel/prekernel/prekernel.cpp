#include <stdint.h>
#include <stddef.h>
#include "multiboot.h"
#include "../memory/PhysicalMemoryManager.h"

#define CHECK_FLAG(flags, bit) ((flags) & (1 <<(bit)))
#define VADDR_TO_PADDR(vaddr) (vaddr - 0xC0000000)
#define PADDR_TO_VADDR(paddr) (paddr + 0xC0000000)
multiboot_info_t* global_mbi;

extern "C" void prekernelSetup  ( unsigned long magic, multiboot_info_t* mbi) 
{

        /*
        * Check Multiboot magic number
        */ 
        if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
        {
                // PANIC!!
                return;
        }

        mbi = PADDR_TO_VADDR(mbi);
        global_mbi = mbi;

        // Setup the physical memory manager immmediatly 
        // Doing so saves the complications of doing it later when 
        // paging is enabled 

        /*
        If we got a memory map from our bootloader we 
        should be parsing it to find out the memory regions available.
        */
        if (CHECK_FLAG(mbi->flags, 6))
        {  
               
                // Calculate total memory size 
                uint32_t RAM_size = 0;
                for(
                        multiboot_memory_map_t* mmap = (multiboot_memory_map_t*) mbi->mmap_addr;
                         (unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length;
                          mmap += mmap->size +sizeof(mmap->size)
                ){
                        RAM_size += mmap->len;
                }
                
                // Call SetupPhysicalMemoryManager at its physical address 
                SetupPhysicalMemoryManager ( (uint32_t)VADDR_TO_PADDR(&kernel_end), RAM_size);
                
                for(
                        multiboot_memory_map_t* mmap = (multiboot_memory_map_t*) mbi->mmap_addr;
                         (unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length;
                          mmap += mmap->size +sizeof(mmap->size)
                ){

                        if(mmap->type == MULTIBOOT_MEMORY_AVAILABLE)
                                deallocate_region(mmap->addr, mmap->len);
                        if(mmap->type == MULTIBOOT_MEMORY_ACPI_RECLAIMABLE)
                                allocate_region(mmap->addr, mmap->len);
                        if(mmap->type == MULTIBOOT_MEMORY_RESERVED)
                                allocate_region(mmap->addr, mmap->len);
                        if(mmap->type == MULTIBOOT_MEMORY_NVS)
                                allocate_region(mmap->addr, mmap->len);
                        if(mmap->type == MULTIBOOT_MEMORY_BADRAM)
                                allocate_region(mmap->addr, mmap->len);

                }
               
               
                // Allocate the kernel
                allocate_region( (uint32_t)&kernel_begin, ( (uint32_t)&kernel_end - (uint32_t)&kernel_begin)- 0xC0000000 );

                // Allocate the memory region below 1MB
                allocate_region(0x0000000, 0x00100000);

        }
        else
        {
                // We didn't get a memory map from our bootloader.
                // PANIC!!!!
                return;
        }

        // allocate a full block for the other boot info!
        BootInfoBlock* BIB = (BootInfoBlock*) allocate_block();

        
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
        // NOTE: Do something with it.. (Store it , process it etc...)
        // printf("- Valid Symbol Table available at 0x%x.\n Tab Size: %d, str Size: %d\n", BootInfo->SymbolTableAddr, BootInfo->SymbolTabSize, BootInfo->SymbolStrSize);
                BIB->ValidSymbolTable = true;
                multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);

        } else{
                BIB->ValidSymbolTable = false;
        }

        /* Is the section header table of ELF valid? */
        if (CHECK_FLAG(mbi->flags, 5))
        {
                  // NOTE: Do something with it.. (Store it , process it etc...)
                BIB->ValidELFHeader = true;
                multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

        }else{
                BIB->ValidELFHeader = false;
        }

        /* Draw diagonal blue line */
        if (CHECK_FLAG (mbi->flags, 12)){
                BIB->EnabledVBE = true;

                   // NOTE: Do something with it.. (Store it , process it etc...)
        } else{
                BIB->EnabledVBE;
        }
}
