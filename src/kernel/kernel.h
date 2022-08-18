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

#include "Memory/memory.h"
#include "Memory/memoryinfo.h"
#include "Memory/paging.h"
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


