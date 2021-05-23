#pragma once 
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../../../libc/include/string.h"
#include "../vga/colors.h"

#define KernelTag "[Kernel]: "
#define AS_KERNEL() (  kterm_setcolor(VGA_COLOR_LIGHT_BLUE),\
                       kterm_write(KernelTag, 10 ), \
                       kterm_resetcolor())



class TextMode {
    public:
        TextMode ();
        ~TextMode();



        void kterm_resetcolor();
        void kterm_setcolor(uint8_t);

        void kterm_putat(char, uint8_t, size_t, size_t);
        void kterm_put(char);
        void kterm_write(const char*, size_t);
        void kterm_writestring(const char*);

        void println( char* data);

    private:

        void kterm_scrollup();

};




