#include "lib/string.h" 
#include "definitions.h"

#include "prekernel/bootstructure.h"

#include "drivers/vga/VBE.h"
#include "drivers/pit/pit.h"

#include "memory/PhysicalMemoryManager.h"
#include "memory/VirtualMemoryManager.h"
#include "memory/KernelHeap.h"
#include "memory/gdt/gdtc.h"

#include "interrupts/idt/idt.h"

#include "io.h"
#include "cpu.h"
#include "serial.h"
#include "time.h"

#include "terminal/kterm.h"

#include "supervisorterminal/superVisorTerminal.h"

extern "C" void kernel_main ();
void ProcessBootInfo();

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
    print_serial("Hello Higher half kernel!\n");

    kterm_init();

    printf("Allocated blocks: %d \n", GetUsedBlocks());

    initGDT();
    init_idt();

    // Enable interrupts
    asm volatile("STI");
   

    initHeap();

    // test heap allocation
    /*
    struct KernelInfo {
        int bar;
        bool foo;
    };

    KernelInfo* MyInfo = (KernelInfo*) malloc(sizeof(KernelInfo));

    MyInfo->bar = 6;
    MyInfo->foo = false;

    free(MyInfo);
    */

    printf("Enable Protected mode and jump to kernel main\n");

	asm volatile("mov %cr0, %eax ");
    asm volatile("or $1, %eax");
    asm volatile("mov %eax, %cr0"); // re-enable protected mode ? 

    pit_initialise();


    kernel_main();
    
}