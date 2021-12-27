#pragma once
extern "C"{
   #include "../libc/include/string.h" 
}


#include "drivers/VGA/VBE.h"
#include "tty/kterm.h"

#include "./bootloader/multiboot.h"
#include "bootcheck.h"
#include "memory/PhysicalMemoryManager.h"

#include "gdt/gdtc.h"
#include "idt/idt.h"

#include "drivers/IO/io.h"
#include "time.h"
#include "cpu.h"
#include "serial.h"
#include "drivers/IO/PCI/pci.h"
#include "ide/ide.h"
#include "./drivers/IO/ata/ataDevice.h"
#include "./PartitionTable/MBR/MasterBootRecord.h"
#include "./filesytems/FAT/BiosParameterBlock.h"
#include "./filesytems/FAT/ExtendBootRecord.h"
#include "./filesytems/FAT/DirectoryEntry.h"
#include "drivers/ACPI/rsdp.h"


#define CHECK_FLAG(flags, bit) ((flags) & (1 <<(bit)))
#define PANIC(message) {  return; }


/* This needs to be moved! */
/**
 *      simple delay function 
 **/
void delay(int t){
    volatile int i,j;
    for(i=0;i<t;i++)
        for(j=0;j<25000;j++)
            asm("NOP");
}
