/*
    Copyright © Nigel Barink 2023
*/
#include "memory/memory.h"
#include "memory/KernelHeap.h"
#include "memory/gdt/gdtc.h"
#include "memory/TaskStateSegment.h"
#include "supervisorterminal/superVisorTerminal.h"
#include "drivers/vga/VBE.h"
#include "pci/pci.h"
#include "drivers/pit/pit.h"
#include "acpi/acpi.h"
#include "i386/processor.h"
#include "terminal/kterm.h"
#include "interrupts/idt.h"
#include "serial.h"
#include "storage/vfs/VFS.h"
#include "../CoreLib/Memory.h"

extern "C"  void LoadGlobalDescriptorTable();
extern "C" void jump_usermode();

extern "C" void kernel ()
{

    init_serial();
    kterm_init();


    setup_tss();
    initGDT();
    initidt();
    LoadGlobalDescriptorTable();
    flush_tss();
    printf("Memory setup complete!\n");

    // Enable interrupts
    asm volatile("STI");

    initHeap();
    pit_initialise();
    ACPI::initialize();

    //PCI::Scan();
    processor::initialize();
    processor::enable_protectedMode();

    VirtualFileSystem::initialize();


#ifdef USERMODE_RELEASE
    // Lets jump into user mode
    jump_usermode();
#else
    startSuperVisorTerminal();
#endif

}