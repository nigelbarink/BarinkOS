#pragma once
extern "C"{
   #include "../libc/include/string.h" 
}


#include "vga/VBE.h"
#include "tty/kterm.h"

#include "./bootloader/multiboot.h"
#include "bootcheck.h"
#include "memory/PhysicalMemoryManager.h"

#include "gdt/gdtc.h"
#include "idt/idt.h"

#include "io.h"
#include "time.h"
#include "cpu.h"
#include "serial.h"
#include "pci.h"
#include "ide/ide.h"


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
