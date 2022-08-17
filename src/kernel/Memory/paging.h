#pragma once 
#include "memory.h"
#include "paging.definitions.h"
#include "../Terminal/kterm.h"
#include "../cpu.h"

extern "C" void loadPageDirectory (uint32_t* addr );
extern "C" void enablePaging();


void IdentityMap();
void InitializePaging();

void Enable();

void AllocatePage(VIRTUAL_ADDRESS, PageDirectoryEntry&);
void FreePage(VIRTUAL_ADDRESS, PageDirectoryEntry&);

void Map(PHYSICAL_ADDRESS, VIRTUAL_ADDRESS, PageDirectoryEntry&);
void Unmap (VIRTUAL_ADDRESS, PageDirectoryEntry&);

