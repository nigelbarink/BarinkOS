#include "idt.h"

IDT_entry idt_table[256];
IDT_ptr idt_ptr;

void set_id_entry (uint8_t num , uint32_t base, uint16_t sel,  uint8_t flags){
    idt_table[num].offset_1 = base & 0xFFFF;
    idt_table[num].selector = sel;
    idt_table[num].zero = 0;
    idt_table[num].type_attr = flags;
    idt_table[num].offset_2 = (base >> 16) & 0xFFFF;

};


void irq_handler (registers regs) {
        kterm_writestring("received interrupt!");
}


void init_idt(){
    // Initialise the IDT pointer
    idt_ptr.length = sizeof(IDT_entry) * 255;
    idt_ptr.base = (uint32_t)&idt_table;


    // TODO: Set everything to zero first

    set_id_entry(0, (uint32_t) irq0 , 0x08, 0x8E);
    set_id_entry(1, (uint32_t) irq1 , 0x08, 0x8E);
    set_id_entry(2, (uint32_t) irq2 , 0x08, 0x8E);
    set_id_entry(3, (uint32_t) irq3 , 0x08, 0x8E);
    set_id_entry(4, (uint32_t) irq4 , 0x08, 0x8E);
    set_id_entry(5, (uint32_t) irq5 , 0x08, 0x8E);
    set_id_entry(6, (uint32_t) irq6 , 0x08, 0x8E);
    set_id_entry(7, (uint32_t) irq7 , 0x08, 0x8E);
    set_id_entry(8, (uint32_t) irq8 , 0x08, 0x8E);
    set_id_entry(9, (uint32_t) irq9 , 0x08, 0x8E);
    set_id_entry(10, (uint32_t) irq10 , 0x08, 0x8E);
    set_id_entry(11, (uint32_t) irq11 , 0x08, 0x8E);
    set_id_entry(12, (uint32_t) irq12 , 0x08, 0x8E);
    set_id_entry(13, (uint32_t) irq13 , 0x08, 0x8E);
    set_id_entry(14, (uint32_t) irq14 , 0x08, 0x8E);
    set_id_entry(15, (uint32_t) irq15 , 0x08, 0x8E);
    set_id_entry(16, (uint32_t) irq16 , 0x08, 0x8E);
    set_id_entry(17, (uint32_t) irq17 , 0x08, 0x8E);
    set_id_entry(18, (uint32_t) irq18 , 0x08, 0x8E);
    set_id_entry(19, (uint32_t) irq19 , 0x08, 0x8E); 
    set_id_entry(20, (uint32_t) irq20 , 0x08, 0x8E);
    set_id_entry(21, (uint32_t) irq21 , 0x08, 0x8E);
    set_id_entry(22, (uint32_t) irq22 , 0x08, 0x8E);
    set_id_entry(23, (uint32_t) irq23 , 0x08, 0x8E);
    set_id_entry(24, (uint32_t) irq24 , 0x08, 0x8E);
    set_id_entry(25, (uint32_t) irq25 , 0x08, 0x8E);
    set_id_entry(26, (uint32_t) irq26 , 0x08, 0x8E);
    set_id_entry(27, (uint32_t) irq27 , 0x08, 0x8E); 
    set_id_entry(28, (uint32_t) irq28 , 0x08, 0x8E);
    set_id_entry(29, (uint32_t) irq29 , 0x08, 0x8E);
    set_id_entry(30, (uint32_t) irq30 , 0x08, 0x8E);
    set_id_entry(31, (uint32_t) irq31 , 0x08, 0x8E);
    
    
    idt_flush((uint32_t)&idt_ptr);
}
