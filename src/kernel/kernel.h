#pragma once
extern "C"{
   #include "../libc/include/string.h" 
}
#include "arch/i386/vga/VBE.h"
#include "arch/i386/tty/kterm.h"

#include "./bootloader/multiboot.h"

#include "bootcheck.h"

#include "arch/i386/gdt/gdtc.h"
#include "arch/i386/idt/idt.h"

#include "io.h"
#include "time.h"
#include "cpu.h"
#include "serial.h"

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
