#pragma once
extern "C" 
{
   #include "Lib/string.h" 
}
#include "definitions.h"

#include "Drivers/VGA/VBE.h"
#include "Terminal/kterm.h"

#include "Memory/PhysicalMemoryManager.h"
#include "Memory/VirtualMemoryManager.h"

#include "Memory/GDT/gdtc.h"
#include "Interrupts/idt/idt.h"

#include "Drivers/PIT/pit.h"
#include "io.h"
#include "cpu.h"
#include "serial.h"

#include "time.h"
#include "SuperVisorTerminal/superVisorTerminal.h"
#include "PreKernel/bootstructure.h"

#define CHECK_FLAG(flag, bit) ( flag & (1 << bit ))
#define PANIC(message) {return;} 

void map_multiboot_info_structure(unsigned long addr);

extern "C" void kernel_main ();


extern "C" uint32_t boot_page_directory;
extern "C" uint32_t multiboot_page_table;


const uint32_t KERNEL_BASE_ADDR = 0xC0000000;