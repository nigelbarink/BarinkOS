#include "gdtc.h"
#include "../../terminal/kterm.h"

#define NULL_SEGMENT          0
#define KERNEL_CODE_SEGMENT   1
#define KERNEL_DATA_SEGMENT   2
#define USER_CODE_SEGMENT     3
#define USER_DATA_SEGMENT     4
#define TASK_STATE_SEGMENT    5

SegmentDescriptor GlobalDescriptorTable[6];
GlobalDescriptorTableDescriptor gdtDescriptor;
tss32 TaskStateSegment = {};



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


      printf("Init GDT!\n");

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

      // Task Segment Descriptor
      add_descriptor(TASK_STATE_SEGMENT, (unsigned long)&TaskStateSegment, sizeof(TaskStateSegment), 0x89, 0x0);
      
      // init Gdt Descriptor
      gdtDescriptor.limit = ((sizeof(SegmentDescriptor ) * 5 ) - 1);
      gdtDescriptor.base = (unsigned int) (&GlobalDescriptorTable);


      LoadGlobalDescriptorTable();

}
