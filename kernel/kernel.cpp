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
#include "storage/vfs/vfs.h"

extern "C"  void LoadGlobalDescriptorTable();
extern "C" void jump_usermode();
extern BootInfoBlock* BIB;

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
    processor::initialize();
    processor::enable_protectedMode();
    printf("Boot device: 0x%x\n", BIB->bootDeviceID);
    unsigned int part3 = BIB->bootDeviceID & 0xFF;
    unsigned int part2 = (BIB->bootDeviceID & 0xFF00) >> 8;
    unsigned int part1 = (BIB->bootDeviceID & 0xFF0000) >> 16;
    unsigned int drive = (BIB->bootDeviceID & 0xFF000000) >> 24;
    if (drive == 0x80 )
        printf("booted from disk!\n");
    if(drive == 0x00)
        printf("booted from floppy disk\n");

    printf("Part1: %d, Part2: %d, Part3: %d\n", part1, part2 , part3);


    VirtualFileSystem::initialize();

    // Try and open hello.txt file
    VirtualFileSystem::OpenFile("a:hello.txt");



    // Try and open grub.cfg file
    VirtualFileSystem::OpenFile("a:boot/grub/grub.cfg");



#ifdef USERMODE_RELEASE
    // Lets jump into user mode
    jump_usermode();
#else
    startSuperVisorTerminal();
#endif

}