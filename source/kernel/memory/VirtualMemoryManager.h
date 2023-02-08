#pragma once 
#include "../terminal/kterm.h"
#include "../cpu.h"
#include "PhysicalMemoryManager.h"


void SetupVMM();

void AllocatePage(uint32_t v_addr );
void FreePage(uint32_t v_addr);

void Immediate_Map(uint32_t vaddr, uint32_t paddr);
void Immediate_Unmap (uint32_t v_addr);

// void Demand_map(uint32_t p_addr, uint32_t v_addr);
// void Demand_Unmap (uint32_t v_addr);
