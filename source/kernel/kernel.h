#pragma once
extern "C" 
{
   #include "../lib/include/string.h" 
}


#include "drivers/VGA/VBE.h"
#include "tty/kterm.h"

#include "./boot/multiboot.h"
#include "bootinfo.h"

#include "memory/memory.h"
#include "memory/memoryinfo.h"
#include "bootcheck.h"

#include "gdt/gdtc.h"
#include "interrupts/idt/idt.h"

#include "drivers/IO/io.h"
#include "time.h"
#include "drivers/pit/pit.h"

#include "cpu.h"
#include "serial.h"
#include "drivers/IO/PCI/pci.h"
#include "drivers/ide/ide.h"
#include "./drivers/IO/ata/ataDevice.h"
#include "./PartitionTable/MBR/MasterBootRecord.h"
#include "./filesystem/FAT/BiosParameterBlock.h"
#include "./filesystem/FAT/ExtendBootRecord.h"
#include "./filesystem/FAT/DirectoryEntry.h"
#include "drivers/ACPI/rsdp.h"


#include "time.h"
#include "sv-terminal/superVisorTerminal.h"

#define CHECK_FLAG(flags, bit) ((flags) & (1 <<(bit)))
#define PANIC(message) {return;} 


