#pragma once 
#include "../terminal/kterm.h"
#include "../time.h"
#include "../drivers/pit/pit.h"
#include "../drivers/ps-2/keyboard.h"
#include "../memory/PhysicalMemoryManager.h"
extern "C" {
    #include "../../lib/include/string.h"
}

void startSuperVisorTerminal();