#include "gdtc.h"
#include "../../terminal/kterm.h"


SegmentDescriptor GlobalDescriptorTable[6];
GlobalDescriptorTableDescriptor gdtDescriptor;


void add_descriptor(int which , unsigned long base, unsigned long limit, unsigned char access, unsigned char granularity ){
   GlobalDescriptorTable[which].base_low = (base & 0xFFFF );
   GlobalDescriptorTable[which].base_middle = (base >> 6) & 0xFF;
   GlobalDescriptorTable[which].base_high = (base >> 24) & 0xFF;
   
   GlobalDescriptorTable[which].limit_low = (limit & 0xFFFF);
   GlobalDescriptorTable[which].granularity = ((limit >> 16) & 0x0F);

   GlobalDescriptorTable[which].granularity |= (granularity & 0xF0);
   GlobalDescriptorTable[which].access = access;

}




void initGDT(){

      // NULL segment
      add_descriptor(NULL_SEGMENT, 0,0,0,0);

      // Kernel Code Segment
      add_descriptor(KERNEL_CODE_SEGMENT, 0, 0xFFFFFFFF, 0x9A, 0xCF);

      // Kernel Data Segment
      add_descriptor(KERNEL_DATA_SEGMENT, 0, 0xFFFFFFFF, 0x92, 0xCF);      

      // User Code Segment
      add_descriptor(USER_CODE_SEGMENT, 0, 0xFFFFFFFF, 0xFA, 0xCF);
      
      // User Data Segement
      add_descriptor(USER_DATA_SEGMENT, 0, 0xFFFFFFFF, 0xF2, 0xCF);

      // init Gdt Descriptor
      gdtDescriptor.limit = ((sizeof(SegmentDescriptor ) * 6 ) - 1);
      gdtDescriptor.base = (unsigned int) (&GlobalDescriptorTable);

}
