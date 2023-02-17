/*
    Copyright © Nigel Barink 2023
*/
extern "C"{
#include "../lib/include/string.h"
}

#include "memory/memory.h"
#include "memory/KernelHeap.h"
#include "memory/gdt/gdtc.h"
#include "memory/TaskStateSegment.h"
#include "supervisorterminal/superVisorTerminal.h"

#include "drivers/vga/VBE.h"
#include "drivers/pci/pci.h"
#include "drivers/pit/pit.h"
#include "drivers/acpi/acpi.h"
#include "drivers/ide/ide.h"

#include "terminal/kterm.h"
#include "interrupts/idt.h"
#include "serial.h"
extern "C"  void LoadGlobalDescriptorTable();
extern "C" void jump_usermode();

void set_protected_bit()
{
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
}

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
    PCI::Scan();

    TestIDEController();

    printf("Enable Protected mode and jump to kernel main\n");

    set_protected_bit();

#ifdef USERMODE_RELEASE
    // Lets jump into user mode
    jump_usermode();
#else
    startSuperVisorTerminal();
#endif

}