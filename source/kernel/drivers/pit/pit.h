#pragma once 
#include <stdint.h>
#include "../IO/io.h"
#define PIT_DATA_0 0x40
#define PIT_DATA_1 0x41
#define PIT_DATA_2 0x42
#define PIT_COMMAND 0x43


extern uint32_t pit_tick;


void pit_initialise();


void get_pit_count();

void set_pit_count();
