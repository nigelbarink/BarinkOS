#pragma once
#include "stdint.h"
#include "stddef.h"
#include "arch/i386/vga/colors.h"

#define AS_KERNEL() (  kterm_writestring("[KERNEL]:"))

extern "C" void kterm_writestring(const char* data );
extern "C" void kterm_putat(char, uint8_t, size_t, size_t);
extern "C" void kterm_put(char);


extern "C" {
    struct __attribute__((__packed__)) IDT_entry {
        uint16_t offset_1;
        uint16_t selector;
        uint8_t zero;
        uint8_t type_attr;
        uint16_t offset_2;
    };

    struct __attribute__((__packed__)) IDT_ptr {
        unsigned short length;
        unsigned long base;
    };

    struct registers {
        uint32_t ds;                  // Data segment selector
        uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
        uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
        uint32_t eip, cs, eflags, useresp, ss; 
    };


    extern void idt_flush(uint32_t); 
    void set_id_entry (uint8_t num , uint32_t base, uint16_t sel,  uint8_t flags);
    void init_idt();

    void irq_handler (registers regs);

    extern void irq0 ();
    extern void irq1 ();
    extern void irq2 ();
    extern void irq3 ();
    extern void irq4 ();
    extern void irq5 ();
    extern void irq6 ();
    extern void irq7 ();
    extern void irq8 ();
    extern void irq9 ();
    extern void irq10 ();
    extern void irq11 ();
    extern void irq12 ();
    extern void irq13 ();
    extern void irq14 ();
    extern void irq15 ();
    extern void irq16 ();
    extern void irq17 ();
    extern void irq18 ();
    extern void irq19 ();
    extern void irq20 ();
    extern void irq21 ();
    extern void irq22 ();
    extern void irq23 ();
    extern void irq24 ();
    extern void irq25 ();
    extern void irq26 ();
    extern void irq27 ();
    extern void irq28 ();
    extern void irq29 ();
    extern void irq30 ();
    extern void irq31 ();
/*
    extern void irq0 ();
    extern void irq1 ();
    extern void irq2 ();
    extern void irq3 ();
    extern void irq4 ();
    extern void irq5 ();
    extern void irq6 ();
    extern void irq7 ();
    extern void irq8 ();
    extern void irq9 ();
    extern void irq10 ();
    extern void irq11 ();
    extern void irq12 ();
    extern void irq13 ();
    extern void irq14 ();
    extern void irq15 ();*/



}
