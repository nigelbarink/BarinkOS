#pragma once
#include <stdint.h>
// Lets assume we have 2 gigabytes of memory 
// NOTE: We should really detect how much memory we have
#define KiloByte 1024 // bytes
#define MegaByte 1048576 // bytes
#define GigaByte 1073741824 // bytes
#define MemorySize 2147483648 // bytes

const uint32_t bitmapSize = MemorySize / 8;

extern void *kernel_begin;
extern void *kernel_end;
struct __attribute__((packed))  PMSegment {
    void* address;
    uint32_t size; 
    PMSegment* Next;
};


static PMSegment pmStart;
static uint32_t AvailablePhysicalMemory;  
static uint32_t AllocatedMemorySize;

void initPhysicalMemoryManager(){
    AvailablePhysicalMemory = MemorySize;
    AllocatedMemorySize = 0;

    // Map the kernel
    PMSegment KernelSegment = PMSegment();
    printf("end of kernel: 0x%x\n", &kernel_end);
    printf("start of kernel: 0x%x\n", &kernel_begin);
    printf("pointer to kernel: 0x%x\n", &KernelSegment);

    pmStart = KernelSegment;
    KernelSegment.address = kernel_begin;
    KernelSegment.size = &kernel_end - &kernel_begin;
    AllocatedMemorySize += KernelSegment.size;
    KernelSegment.Next = 0;

    // make sure We allocate space for ourselves 
    /*PMSegment start =  PMSegment();
    start.address = KernelSegment.address + KernelSegment.size ;
    start.size = (MemorySize /  sizeof(PMSegment) ) +  sizeof (uint32_t) * 2;
    */

    //KernelSegment.Next = &start;
    //AllocatedMemorySize += start.size;
}

void printMemorySegments() 
{
    printf("Memory Segments:\n");
    printf( "Start Segment: [addr: 0x%x, size: 0x%x, Next: 0x%x]\n" ,pmStart.address, pmStart.size, pmStart.Next);
    printf("----------------------------\n");
}


void pmem_free (void* address){
    
    PMSegment* before = 0;
    PMSegment* current = &pmStart;

    printf("Address of pmStart : 0x%x\n", pmStart);
    printf("Looking for segment with address: 0x%x \n", address );
    printf("Address of pmStart(a.k.a current) : 0x%x \n", current);
    while( current ) 
    {
        //printf("address of current segment 0x%x\n", current->address );
        if ( current->address == address ) {
            // this is the address we want to free
            printf("Segment found!! Segment address: 0x%x \n", current->address);
            if ( current->Next && before ){
                before->Next = current->Next;
            }else{
                before->Next = 0;
            }
            // TODO: Clean memory [ Something like memset to zeroes]  
            printf("Removing segment of size: 0x%x\n", current->size);
            AllocatedMemorySize -= current->size;
            break;
        }

        before = current;
        current = current->Next;
        
    }

}

void* pmem_alloc ( uint32_t size ){
    // Get the last segment
    PMSegment* lastSegment = &pmStart;

    while (lastSegment ) {
        if( lastSegment->Next == 0){
            break;
        }
        lastSegment = lastSegment->Next;
    }   

    printf("LastSegment is for address: 0x%x\n", lastSegment->address);


    // So now we have the last segment available
    PMSegment newSegment =  PMSegment();
    newSegment.address = (PMSegment*)((uint32_t)lastSegment->address + lastSegment->size +1);

    printf("NewSegment for Address: 0x%x\n", newSegment.address);

    newSegment.size = size;

    lastSegment->Next = &newSegment;

    if ( AllocatedMemorySize + newSegment.size > AvailablePhysicalMemory){
        // No segment available of this size
        /// ERROR!!
        return 0;
    }
    AllocatedMemorySize += newSegment.size;

    return newSegment.address;
    

}