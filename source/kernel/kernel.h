#pragma once
extern "C" 
{
   #include "lib/string.h" 
}
#include "definitions.h"

#include "drivers/vga/VBE.h"
#include "terminal/kterm.h"

#include "memory/PhysicalMemoryManager.h"
#include "memory/VirtualMemoryManager.h"

#include "memory/gdt/gdtc.h"
#include "interrupts/idt/idt.h"

#include "drivers/pit/pit.h"
#include "io.h"
#include "cpu.h"
#include "serial.h"

#include "time.h"
#include "supervisorterminal/superVisorTerminal.h"
#include "prekernel/bootstructure.h"

#define CHECK_FLAG(flag, bit) ( flag & (1 << bit ))
#define PANIC(message) {return;} 

void map_multiboot_info_structure(unsigned long addr);

extern "C" void kernel_main ();


extern "C" uint32_t boot_page_directory;
extern "C" uint32_t multiboot_page_table;


const uint32_t KERNEL_BASE_ADDR = 0xC0000000;