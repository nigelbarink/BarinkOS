#pragma once 
#include "PhysicalMemoryManager.h"
#include "../terminal/kterm.h"
#include "../cpu.h"

void AllocatePage(uint32_t v_addr );
void FreePage(uint32_t v_addr);

void Map(uint32_t p_addr, uint32_t v_addr);
void Unmap (uint32_t v_addr);
