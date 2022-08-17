#pragma once 
#include "../Terminal/kterm.h"
#include "../time.h"
#include "../Drivers/PIT/pit.h"
#include "../Drivers/PS-2/keyboard.h"
#include "../Memory/memory.h"
#include "../bootinfo.h"

void startSuperVisorTerminal(BootInfo * );