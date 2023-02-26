#pragma once

#include <stdint.h>
#include <stddef.h>
#include "../drivers/vga/colors.h"
#include "../drivers/pic/pic.h"

#include "../terminal/kterm.h"


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
    void initidt();

    void irq_handler (registers regs);

    void irs_handler (registers* regs);

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
    extern void irs50();




}
