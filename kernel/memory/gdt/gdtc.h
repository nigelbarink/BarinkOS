#pragma once
#include <stdint.h>


#define NULL_SEGMENT          0
#define KERNEL_CODE_SEGMENT   1
#define KERNEL_DATA_SEGMENT   2
#define USER_CODE_SEGMENT     3
#define USER_DATA_SEGMENT     4
#define TASK_STATE_SEGMENT    5



struct SegmentDescriptor {
   unsigned short limit_low;
   unsigned short base_low;
   unsigned char base_middle;
   unsigned char access;
   unsigned char granularity;
   unsigned char base_high;
}__attribute__((packed));

extern SegmentDescriptor GlobalDescriptorTable[6];

struct GlobalDescriptorTableDescriptor{
   unsigned short limit;
   unsigned int base;
}__attribute__((packed)) ;



void add_descriptor(int which , unsigned long base, unsigned long limit, unsigned char access, unsigned char granularity );


void initGDT();
