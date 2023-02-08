
extern "C" 
{
   #include "../lib/include/string.h" 
}

#include "prekernel/bootstructure.h"

#include "memory/memory.h"
#include "memory/memoryinfo.h"
#include "memory/memory.h"
#include "memory/VirtualMemoryManager.h"
#include "memory/KernelHeap.h"
#include "memory/gdt/gdtc.h"

#include "supervisorterminal/superVisorTerminal.h"

#include "drivers/io/io.h"
#include "drivers/vga/VBE.h"
#include "drivers/pci/pci.h"
#include "drivers/pit/pit.h"


#include "terminal/kterm.h"

#include "prekernel/multiboot.h"
#include "bootinfo.h"

#include "bootcheck.h"

#include "interrupts/idt/idt.h"
#include "time.h"
#include "cpu.h"
#include "serial.h"
#include "time.h"
#include "definitions.h"




/*
    Copyright © Nigel Barink 2023
*/

extern "C" void kernel_main ()
{
    /*
     * Show a little banner for cuteness
     */
    printf("|===    BarinkOS       ===|\n");
    startSuperVisorTerminal();
}   


extern "C" void early_main()
{
    init_serial();

    kterm_init();
    printf("Allocated blocks: 0x%x \n", GetUsedBlocks());

    initGDT();
    init_idt();
    
    // Enable interrupts
    asm volatile("STI");
    
    initHeap(); 

    printf("Enable Protected mode and jump to kernel main\n");


    // Set the protected bit of control register 0 
    // this will put the CPU into protected mode
    // NOTE: This should really be a assembly procedure 
    // We cant directly write to control register 0 
    // therefor we copy the value of control register 0 into eax
    // once we are done manipulating the value we write the value in 
    // eax back to control register 0 

	asm volatile("mov %cr0, %eax ");
    asm volatile("or $1, %eax");
    asm volatile("mov %eax, %cr0");

    pit_initialise();


    kernel_main();
    
}
