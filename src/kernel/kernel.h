#pragma once 
extern "C" {
    #include "../libc/include/string.h"
    #include "arch/i386/tty/kterm.h"
}

#include "multiboot.h"
#include "arch/i386/idt/idt.h"
#include "MMU.h"
#include "io.h"
#include "time.h"


#define CHECK_FLAG(flags, bit) ((flags) & (1 <<(bit)))