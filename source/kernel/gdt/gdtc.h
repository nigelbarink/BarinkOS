#include <stdint.h>


struct SegmentDescriptor {
   unsigned short limit_low;
   unsigned short base_low;
   unsigned char base_middle;
   unsigned char access;
   unsigned char granularity;
   unsigned char base_high;
}__attribute__((packed));


struct GlobalDescriptorTableDescriptor{
   unsigned short limit;
   unsigned int base;
}__attribute__((packed));

extern SegmentDescriptor GlobalDescriptorTable[];
extern GlobalDescriptorTableDescriptor gdtDescriptor;


void add_descriptor(int which , unsigned long base, unsigned long limit, unsigned char access, unsigned char granularity );


extern "C" void LoadGlobalDescriptorTable();
void initGDT();
