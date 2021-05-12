#include "kterm.h"
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t kterm_row;
size_t kterm_column;
uint8_t kterm_color;
uint16_t* kterm_buffer;


static inline uint8_t vga_entry_color( enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry (unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

void kterm_init () { 
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


void kterm_resetcolor(){
     kterm_color = vga_entry_color ( VGA_COLOR_LIGHT_GREY , VGA_COLOR_BLACK);
}

void kterm_setcolor(uint8_t color){
    kterm_color = color;
}

void kterm_putat (char c, uint8_t color, size_t x, size_t y ) {
    const size_t index = y * VGA_WIDTH + x;

    kterm_buffer[index] = vga_entry(c, color);

}


/**
 *  With the help from:
 * https://whiteheadsoftware.dev/operating-systems-development-for-dummies/
 **/ 
void kterm_scrollup(){
    size_t i ;
    for(i=0; i < (VGA_WIDTH * VGA_HEIGHT - VGA_WIDTH); i++)
        kterm_buffer[i] = kterm_buffer[i+VGA_WIDTH];

    for( i=0; i< VGA_WIDTH; i++)
        kterm_buffer[(VGA_HEIGHT -1) * VGA_WIDTH + i ] = vga_entry(' ', kterm_color); 

}

void kterm_put (char c) {
 
    if(++kterm_column == VGA_WIDTH || c == '\n' ) {
        kterm_column = 0;
        if(kterm_row == VGA_HEIGHT-1) {       
            kterm_scrollup();
        } else {
            kterm_row ++;
        }
            
    }
    
    if(c == '\n') return;
    kterm_putat ( c, kterm_color, kterm_column, kterm_row);
    

}

void kterm_write(const char* data, size_t size) {
    for(size_t i = 0; i < size; i++){
        kterm_put(data[i]);
    }
}   

void kterm_writestring(const char* data ){
    AS_KERNEL();
    kterm_write(data, strlen(data));
}
