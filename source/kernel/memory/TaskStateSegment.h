#pragma once
#include "gdt/gdtc.h"
#include "../../lib/include/string.h"

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


TaskStateSegment tss0 = {};

inline void flush_tss(){
     asm volatile("mov (5 * 8) |0 , %eax; ltr %ax");
}


void setup_tss(){


    // ensure the tss is zero'd
    //memset((void*)&tss0, 0, sizeof(tss0));
    tss0.ss0 = (uint32_t) &(GlobalDescriptorTable[KERNEL_DATA_SEGMENT]);
    uint32_t esp_addr =0 ; 
    asm volatile ("movl %%esp, %0" : "=a"(esp_addr));
    tss0.esp0 = esp_addr;


    // Task Segment Descriptor
    add_descriptor(TASK_STATE_SEGMENT, (unsigned long)&tss0, sizeof(tss0), 0x89, 0x0);
    flush_tss();
}

