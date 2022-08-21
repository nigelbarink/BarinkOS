#include "VirtualMemoryManager.h"

extern "C" void loadPageDirectory (uint32_t* addr );
extern "C" void enablePaging();

void AllocatePage(uint32_t vaddr)
{

}

void FreePage(uint32_t vaddr )
{

}

void Map (  uint32_t vaddr, uint32_t paddr)
{

}

void Unmap(uint32_t vaddr)
{
    // NOTE: I will implement lazy unmapping for now 
}
