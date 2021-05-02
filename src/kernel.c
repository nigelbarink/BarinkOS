#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "vga_colors.h"
#include "string.h"
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t kterm_row;
size_t kterm_column;
uint8_t kterm_color;
uint16_t* kterm_buffer;


static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry (unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

void init_kterm () { 
    kterm_row = 0;
    kterm_column = 0;
    kterm_color = vga_entry_color ( VGA_COLOR_LIGHT_GREY , VGA_COLOR_BLACK);
    kterm_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++ ){
        for( size_t x = 0; x < VGA_WIDTH; x++){
            const size_t index = y * VGA_WIDTH + x;
            kterm_buffer[index] = vga_entry(' ', kterm_color);

        }
    }
}


void kterm_setcolor(uint8_t color){
    kterm_color = color;
}

void kterm_putat (char c, uint8_t color, size_t x, size_t y ) {
    const size_t index = y * VGA_WIDTH + x;

    kterm_buffer[index] = vga_entry(c, color);

}

void kterm_put (char c) {
    // add newline support 
    if ( c == '\n'){
        kterm_column = 0;
        kterm_row++;
        return;
    }

    kterm_putat ( c, kterm_color, kterm_column, kterm_row);
    if(kterm_column++ == VGA_WIDTH ){
        kterm_column = 0;
        if(kterm_row++ == VGA_HEIGHT)
            kterm_row = 0;
    }

}

void kterm_write(const char* data, size_t size) {
    for(size_t i = 0; i < size; i++){
        kterm_put(data[i]);
    }
}   


void kterm_writestring(const char* data ){
    kterm_write(data, strlen(data));
}


void kernel_main (void) {
    /** initialize terminal interface */ 
    init_kterm();

    kterm_writestring("K: Hello world!\n");
    kterm_writestring("K: We got newline support!");
    
}   