#pragma once
extern "C"{
   #include "../libc/include/string.h" 
}
#include "vga/VBE.h"
#include "tty/kterm.h"

#include "./bootloader/multiboot.h"
#include "bootcheck.h"
#include "memory/physical/PhysicalMemoryManager.h"
#include "memory/frames/PageFrameAllocator.h"

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


