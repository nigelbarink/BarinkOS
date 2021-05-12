#pragma once
#include "stdint.h"

extern "C" void kterm_writestring(const char* data );

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

    extern void irs0 ();
    extern void irs1 ();
    extern void irs2 ();
    extern void irs3 ();
    extern void irs4 ();
    extern void irs5 ();
    extern void irs6 ();
    extern void irs7 ();
    extern void irs8 ();
    extern void irs9 ();
    extern void irs10 ();
    extern void irs11 ();
    extern void irs12 ();
    extern void irs13 ();
    extern void irs14 ();
    extern void irs15 ();
    extern void irs16 ();
    extern void irs17 ();
    extern void irs18 ();
    extern void irs19 ();
    extern void irs20 ();
    extern void irs21 ();
    extern void irs22 ();
    extern void irs23 ();
    extern void irs24 ();
    extern void irs25 ();
    extern void irs26 ();
    extern void irs27 ();
    extern void irs28 ();
    extern void irs29 ();
    extern void irs30 ();
    extern void irs31 ();

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



}
