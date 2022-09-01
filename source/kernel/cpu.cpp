#include "cpu.h"


uint32_t GetEFLAGS()
{
    uint32_t EFLAGS = 0;
    asm volatile ("pushfl;" "movl 4(%%esp), %%edx" : "=d"(EFLAGS));
    return EFLAGS;
}


uint32_t GetCR0()
{   
    uint32_t cr0_value;
    asm volatile ("movl %%cr0, %%edx" : "=d"(cr0_value));
    return cr0_value;

}


uint32_t GetCR2(){
    uint32_t cr2_value;
    __asm__ volatile("movl %%cr2, %%edx": "=d"(cr2_value));
    return cr2_value;
}


uint32_t GetCR3(){
    uint32_t cr3_value;
    __asm__ volatile("movl %%cr3, %%edx": "=d"(cr3_value));
    return cr3_value;
}


uint32_t GetCR4(){
    uint32_t cr4_value;
    __asm__ volatile("movl %%cr4, %%edx": "=d"(cr4_value));
    return cr4_value;
}