#pragma once 
#include "../tty/kterm.h"
#include "../time.h"
#include "../drivers/pit/pit.h"
#include "../keyboard/keyboard.h"
#include "../memory/memory.h"
#include "../bootinfo.h"

void startSuperVisorTerminal(BootInfo * );