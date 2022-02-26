#pragma once
extern "C" 
{
   #include "../libc/include/string.h" 
}

#include "definitions.h"

#include "vga/VBE.h"
#include "tty/kterm.h"

#include "./bootloader/multiboot.h"
#include "bootinfo.h"

#include "memory/memory.h"
#include "memory/memoryinfo.h"
#include "bootcheck.h"

#include "gdt/gdtc.h"
#include "idt/idt.h"

#include "pit.h"
#include "io.h"
#include "cpu.h"
#include "serial.h"

#include "time.h"
#include "sv-terminal/superVisorTerminal.h"

#define CHECK_FLAG(flags, bit) ((flags) & (1 <<(bit)))
#define PANIC(message) {return;} 


