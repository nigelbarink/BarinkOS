#pragma once
extern "C" 
{
   #include "Lib/string.h" 
}

#include "definitions.h"

#include "Drivers/VGA/VBE.h"
#include "Terminal/kterm.h"

#include "multiboot.h"
#include "bootinfo.h"

#include "Memory/PhysicalMemoryManager.h"
#include "Memory/memoryinfo.h"
#include "Memory/VirtualMemoryManager.h"
#include "KernelLauncher/bootcheck.h"

#include "Memory/GDT/gdtc.h"
#include "Interrupts/idt/idt.h"

#include "Drivers/PIT/pit.h"
#include "io.h"
#include "cpu.h"
#include "serial.h"

#include "time.h"
#include "SuperVisorTerminal/superVisorTerminal.h"

#define CHECK_FLAG(flag, bit) ( flag & (1 << bit ))
#define PANIC(message) {return;} 


void map_multiboot_info_structure(unsigned long addr);

extern "C" void kernel_main (BootInfo* bootinfo);

extern "C" const void* kernel_begin;
extern "C" const void* kernel_end;

extern "C" uint32_t boot_page_directory;
extern "C" uint32_t multiboot_page_table;


const uint32_t KERNEL_BASE_ADDR = 0xC0000000;