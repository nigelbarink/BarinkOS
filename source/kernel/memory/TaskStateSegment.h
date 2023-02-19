#pragma once
#include "gdt/gdtc.h"
#include <CoreLib/Memory.h>

struct TaskStateSegment {
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
    // everythinge else is unused 
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint16_t trap;
	uint16_t iomap_base;
}__attribute__((packed));


TaskStateSegment tss0 ={};

inline void flush_tss()
{
     asm volatile("mov $0x2B, %ax ; ltr %ax");
}

void setup_tss(){
    // ensure the tss is zero'd
    memset((void*)&tss0, 0, sizeof(tss0));
    tss0.ss0 =  (uint32_t) &GlobalDescriptorTable[KERNEL_DATA_SEGMENT];
     
    extern uint32_t stack_top;
    tss0.esp0 = (unsigned long)&stack_top;

    // Task Segment Descriptor

    uint32_t address = (unsigned long) &tss0;
    uint32_t size = sizeof(tss0);
    uint32_t limit = (address + size );

    add_descriptor(TASK_STATE_SEGMENT, address, limit- 1, 0xE9, 0x0);
}

